#include "JBNativeFileWatcher.h"
#include "JBFileWatcher.h"
#include "JBFileWatcherNotificationSink.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QThread>

using namespace JBFileWatcherUtils;

JBNativeFileWatcher::JBNativeFileWatcher(QObject *parent)
    : JBPluggableFileWatcher(parent), myLastChangedPathsLock(new QMutex()) {
    myNotificationSink = nullptr;
    myLastChangedPaths.resize(2);
    myIsActive = false;
    myIsSendingRoots = false;
}

JBNativeFileWatcher::~JBNativeFileWatcher() {
}

void JBNativeFileWatcher::initialize(JBFileWatcherNotificationSink *sink) {
    Q_ASSERT(sink);
    myNotificationSink = sink;
    myExecutable = executable();

    myStartAttemptCount = 0;
    myIsShuttingDown = false;
    mySettingRoots = 0;
    myRecursiveWatchRoots.clear();
    myFlatWatchRoots.clear();
    myIgnoredRoots.clear();
    resetChangedPaths();
    myLastOp = WatcherOp::UNKNOWN;
    myLines.clear();

    QFileInfo info(myExecutable);
    if (!info.isFile()) {
        notifyOnFailure("watcher.exe.not.found");
    } else if (!info.isExecutable()) {
        notifyOnFailure("watcher.exe.not.exe");
    } else if (!startupProcess()) {
        notifyOnFailure("watcher.failed.to.start");
    }

    myIsActive = true;
}

void JBNativeFileWatcher::dispose() {
    myIsShuttingDown = true;
    shutdownProcess();

    myIsActive = false;
}

bool JBNativeFileWatcher::isActive() const {
    return myIsActive.loadRelaxed();
}

bool JBNativeFileWatcher::isSendingRoots() const {
    return myIsSendingRoots.loadRelaxed();
}

bool JBNativeFileWatcher::isSettingRoots() const {
    return isOperational() && mySettingRoots.loadRelaxed() > 0;
}

void JBNativeFileWatcher::setWatchRoots(const QStringList &recursive, const QStringList &flat) {
    setWatchRootsCore(recursive, flat, false);
}

void JBNativeFileWatcher::waitForRootsSet() {
    while (isSettingRoots()) {
        qApp->processEvents();
    }
}

bool JBNativeFileWatcher::startupProcess(bool restart) {
    if (myIsShuttingDown.loadRelaxed()) {
        return true;
    }

    if (myStartAttemptCount++ > MAX_PROCESS_LAUNCH_ATTEMPT_COUNT) {
        notifyOnFailure("watcher.bailed.out.10x");
        return false;
    }

    if (restart && !shutdownProcess()) {
        return false;
    }

    jbDebug() << "[Watcher] Starting file watcher:" << myExecutable;

    if (!startProcess(myExecutable)) {
        return false;
    }

    if (restart) {
        if (!myRecursiveWatchRoots.isEmpty() || !myFlatWatchRoots.isEmpty()) {
            jbDebug() << "[Watcher] Restart watcher and set paths again";
            setWatchRootsCore(myRecursiveWatchRoots, myFlatWatchRoots, true);
        }
    }

    return true;
}

bool JBNativeFileWatcher::shutdownProcess() {
    if (myProcess->state() == QProcess::Running) {
        if (!writeLine(EXIT_COMMAND)) {
            return false;
        }
        if (!myProcess->waitForFinished(500)) {
            jbWarning() << "[Watcher] File watcher is still alive, doing a force quit.";
            if (!killProcess()) {
                notifyOnFailure("watcher.failed.to.shut");
                return false;
            }
        }
    }
    return true;
}

void JBNativeFileWatcher::setWatchRootsCore(QStringList recursive, QStringList flat, bool restart) {
    if (!restart && myRecursiveWatchRoots == recursive && myFlatWatchRoots == flat) {
        myNotificationSink->notifyManualWatchRoots(this, myIgnoredRoots);
        return;
    }

    mySettingRoots++;

    myRecursiveWatchRoots = recursive;
    myFlatWatchRoots = flat;

    QStringList ignored;

    if (SystemInfo::isWindows()) {
        recursive = screenUncRoots(recursive, ignored);
        flat = screenUncRoots(flat, ignored);
    }

    myIgnoredRoots = ignored;
    myNotificationSink->notifyManualWatchRoots(this, ignored);

    // Start send comand line
    myIsSendingRoots = true;

    bool flag = true;
    if (writeLine(ROOTS_COMMAND)) {
        for (auto it = recursive.begin(); it != recursive.end(); ++it) {
            const QString &path = *it;
            if (!writeLine(path)) {
                flag = false;
                break;
            }
        }
        if (flag) {
            for (auto it = flat.begin(); it != flat.end(); ++it) {
                const QString &path = *it;
                if (!writeLine('|' + path)) {
                    flag = false;
                    break;
                }
            }
        }
    }
    if (flag) {
        flag &= writeLine("#");
    }
    if (!flag) {
        jbWarning() << "[Watcher] Error setting roots.";
    }

    // Send command line over
    myIsSendingRoots = false;
}

QStringList JBNativeFileWatcher::screenUncRoots(const QStringList &roots, QStringList &ignored) {
    QStringList filtered;
    for (auto it = roots.begin(); it != roots.end(); ++it) {
        const QString &root = *it;
        if (OSAgnosticPathUtil::isUncPath(root)) {
            ignored.append(root);
        } else {
            filtered.append(root);
        }
    }
    return filtered;
}

QString JBNativeFileWatcher::executable() const {
    return FSNotifierExecutable();
}
void JBNativeFileWatcher::resetChangedPaths() {
    QMutexLocker locker(myLastChangedPathsLock.data());

    myLastChangedPathIndex = 0;
    myLastChangedPaths[0] = "";
    myLastChangedPaths[1] = "";
}

bool JBNativeFileWatcher::isRepetition(const QString &path) {
    QMutexLocker locker(myLastChangedPathsLock.data());

    const int length = myLastChangedPaths.size();
    for (int i = 0; i < length; ++i) {
        int last = myLastChangedPathIndex - i - 1;
        if (last < 0) {
            last += length;
        }
        const QString &lastChangedPath = myLastChangedPaths[last];
        if (!lastChangedPath.isEmpty() && !lastChangedPath.compare(path)) {
            return true;
        }
    }

    myLastChangedPaths[myLastChangedPathIndex++] = path;
    if (myLastChangedPathIndex == length) {
        myLastChangedPathIndex = 0;
    }

    return false;
}

void JBNativeFileWatcher::notifyProcessTerminated(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitStatus)

    jbDebug() << "[Watcher] Watcher terminated with exit code" << exitCode;

    if (!startupProcess(true)) {
        shutdownProcess();
        jbWarning() << "[Watcher] Watcher terminated and attempt to restart has failed. Exiting "
                       "watching thread.";
    }
}

void JBNativeFileWatcher::notifyTextAvailable(const QString &line,
                                              QProcess::ProcessChannel channel) {
    if (channel == QProcess::StandardError) {
        jbWarning() << line;
        return;
    }

    if (myLastOp == WatcherOp::UNKNOWN) {
        WatcherOp watcherOp = StringToWatcherOp(line);
        if (watcherOp == WatcherOp::UNKNOWN) {
            jbWarning().noquote() << "[Watcher] Illegal watcher command: \'" + line + "\'";
            return;
        }
        if (watcherOp == WatcherOp::GIVEUP) {
            jbDebug() << "[Watcher] Output: GiveUp";
            notifyOnFailure("watcher.gave.up");
            myIsShuttingDown = true;
        } else if (watcherOp == WatcherOp::RESET) {
            jbDebug() << "[Watcher] Output: Reset";
            myNotificationSink->notifyReset("");
        } else {
            myLastOp = watcherOp;
        }
    } else if (myLastOp == WatcherOp::MESSAGE) {
        notifyOnFailure(MessageToFailureReasonString(line));
        myLastOp = WatcherOp::UNKNOWN;
    } else if (myLastOp == WatcherOp::REMAP || myLastOp == WatcherOp::UNWATCHEABLE) {
        if (line == '#') {
            if (myLastOp == WatcherOp::REMAP) {
                jbDebug() << "[Watcher] Output: Remap";
                processRemap();
            } else {
                jbDebug() << "[Watcher] Output: Unwatchable";
                mySettingRoots--;
                processUnwatchable();
            }
            myLines.clear();
            myLastOp = WatcherOp::UNKNOWN;
        } else {
            myLines.append(line);
        }
    } else {
        QString path = line.simplified();
        processChange(path, myLastOp);
        myLastOp = WatcherOp::UNKNOWN;
    }
}

void JBNativeFileWatcher::notifyErrorOccured(QProcess::ProcessError error) {
    Q_UNUSED(error)
}

void JBNativeFileWatcher::notifyOnFailure(const QString &reason) {
    myNotificationSink->notifyUserOnFailure(reason);
}

void JBNativeFileWatcher::processRemap() {
    QList<QPair<QString, QString>> map;
    for (int i = 0; i < myLines.size() - 1; i += 2) {
        map.append({myLines.at(i), myLines.at(i + 1)});
    }
    myNotificationSink->notifyMapping(map);
}

void JBNativeFileWatcher::processUnwatchable() {
    myIgnoredRoots.append(myLines);
    myNotificationSink->notifyManualWatchRoots(this, myLines);
}

void JBNativeFileWatcher::processChange(const QString &path, WatcherOp op) {
    if (SystemInfo::isWindows()) {
        if (op == WatcherOp::RECDIRTY) {
            myNotificationSink->notifyReset(path);
            return;
        }
    }

    if ((op == WatcherOp::CHANGE || op == WatcherOp::STATS) && isRepetition(path)) {
        return;
    }

    if (SystemInfo::isMac()) {
        // path = Normalizer.normalize(path, Normalizer.Form.NFC);
    }

    switch (op) {
    case WatcherOp::STATS:
    case WatcherOp::CHANGE:
        myNotificationSink->notifyDirtyPath(path);
        break;

    case WatcherOp::CREATE:
    case WatcherOp::DELETE:
        myNotificationSink->notifyPathCreatedOrDeleted(path);
        break;

    case WatcherOp::DIRTY:
        myNotificationSink->notifyDirtyDirectory(path);
        break;

    case WatcherOp::RECDIRTY:
        myNotificationSink->notifyDirtyPathRecursive(path);
        break;

    default:
        jbWarning() << "[Watcher] Unexpected op:" << WatcherOpToString(op);
    }
}

QString JBNativeFileWatcher::fsnotifier_path =
#ifdef Q_OS_WINDOWS
    "fsnotifier.exe"
#else
    "fsnotifier"
#endif
    ;

QString JBNativeFileWatcher::FSNotifierExecutable() {
    return fsnotifier_path;
}

void JBNativeFileWatcher::setFsNotifierExecutablePath(const QString &path) {
    fsnotifier_path = path;
}
