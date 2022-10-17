#include "JBLocalFileSystemTimer.h"
#include "JBLocalFileSystem.h"

#include <QTimerEvent>

JBLocalFileSystemTimer::JBLocalFileSystemTimer(JBLocalFileSystem *fs, QObject *parent)
    : QObject(parent), fs(fs), lock(new QMutex()) {
    timerId = -1;
}

JBLocalFileSystemTimer::~JBLocalFileSystemTimer() {
    if (isRunning()) {
        stop();
    }
}

void JBLocalFileSystemTimer::start() {
    jbDebug() << "[Local FS] Dirty path timer thread starts.";
    if (!isRunning()) {
        timerId = startTimer(500);
    }
}

void JBLocalFileSystemTimer::stop() {
    if (isRunning()) {
        killTimer(timerId);
        timerId = -1;
    }
    jbDebug() << "[Local FS] Dirty path timer thread stops.";
}

bool JBLocalFileSystemTimer::isRunning() const {
    return timerId.loadRelaxed() >= 0;
}

bool JBLocalFileSystemTimer::storeRefreshStatusToFiles() {
    // Run in any thread
    QMutexLocker locker(lock.data());

    auto watcher = fs->fileWatcher();
    if (watcher->isOperational()) {
        auto dirtyPaths = watcher->getDirtyPaths();

        markPathsDirty(dirtyPaths.dirtyPaths());
        markFlatDirsDirty(dirtyPaths.dirtyDirectories());
        markRecursiveDirsDirty(dirtyPaths.dirtyPathsRecursive());

        return !dirtyPaths.isEmpty();
    }
    return false;
}

void JBLocalFileSystemTimer::markPathsDirty(const QStringList &paths) {
    lastDirtyPaths = paths;
}

void JBLocalFileSystemTimer::markFlatDirsDirty(const QStringList &paths) {
    lastDirtyFlatDirs = paths;
}

void JBLocalFileSystemTimer::markRecursiveDirsDirty(const QStringList &paths) {
    lastDirtyRecursiveDirs = paths;
}

void JBLocalFileSystemTimer::timerEvent(QTimerEvent *event) {
    if (event->timerId() == timerId) {
        if (storeRefreshStatusToFiles()) {
            emit afterMarkDirtyCallback(lastDirtyPaths, lastDirtyFlatDirs, lastDirtyRecursiveDirs);
        }
    }
}
