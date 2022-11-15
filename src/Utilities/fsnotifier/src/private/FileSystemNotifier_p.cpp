#include "FileSystemNotifier_p.h"
#include "../FileSystemNotifier.h"
#include "../core/JBFileWatchNamespace.h"

using namespace JBFileWatcherUtils;

#include <QCoreApplication>

FileSystemNotifierPrivate::FileSystemNotifierPrivate(FileSystemNotifier *q) : q(q), fs(nullptr) {
    hasChangeEvent = false;
    rootsNeedUpdate = false;
    separatorSystemDependent = true;
}

FileSystemNotifierPrivate::~FileSystemNotifierPrivate() {
}

void FileSystemNotifierPrivate::init() {
    fs = new JBLocalFileSystem(q);

    q->connect(fs, &JBLocalFileSystem::pathsDirty, q, [this](const QStringList &paths) { //
        emit q->changed((separatorSystemDependent ? listPathToNativeSeparators
                                                  : listPathFromNativeSeparators)(paths));
    });
    q->connect(fs, &JBLocalFileSystem::flatDirsDirty, q, [this](const QStringList &paths) { //
        emit q->changed((separatorSystemDependent ? listPathToNativeSeparators
                                                  : listPathFromNativeSeparators)(paths));
    });
    q->connect(fs, &JBLocalFileSystem::recursivePathsDirty, q, [this](const QStringList &paths) { //
        emit q->renamed((separatorSystemDependent ? listPathToNativeSeparators
                                                  : listPathFromNativeSeparators)(paths));
    });
    q->connect(fs, &JBLocalFileSystem::failureOccured, q, [this](const QString &reason) { //
        emit q->failed(reason);
    });
}

void FileSystemNotifierPrivate::changeRecursivePathsToAdd(const QString &path, int n) {
    if (n == 0) {
        return;
    }
    const auto &newPath = QDir::toNativeSeparators(path);
    auto it2 = recursivePathsToAdd.find(newPath);
    if (it2 == recursivePathsToAdd.end()) {
        it2 = recursivePathsToAdd.insert(newPath, 0);
    }
    it2.value() += n;
    if (it2.value() == 0) {
        recursivePathsToAdd.erase(it2);
    }
}

void FileSystemNotifierPrivate::changeFlatPathsToAdd(const QString &path, int n) {
    if (n == 0) {
        return;
    }
    const auto &newPath = QDir::toNativeSeparators(path);
    auto it2 = flatPathsToAdd.find(newPath);
    if (it2 == flatPathsToAdd.end()) {
        it2 = flatPathsToAdd.insert(newPath, 0);
    }
    it2.value() += n;
    if (it2.value() == 0) {
        flatPathsToAdd.erase(it2);
    }
}

void FileSystemNotifierPrivate::postChange() {
    hasChangeEvent = true;
    QCoreApplication::postEvent(q, new QTimerEvent(-1));
}

void FileSystemNotifierPrivate::commitChange() {
    rootsNeedUpdate = false;

    if (!recursivePathsToAdd.isEmpty() || !flatPathsToAdd.isEmpty()) {
        QList<JB::WatchRequest> requestsToRemove;
        QStringList recursivePaths;
        QStringList flatPaths;
        for (auto it = recursivePathsToAdd.begin(); it != recursivePathsToAdd.end(); ++it) {
            const QString &path = it.key();
            int cnt = it.value();
            if (cnt < 0) {
                for (int i = 0; i < -cnt; ++i) {
                    requestsToRemove.append(JB::WatchRequest(path, true));
                }
            } else {
                for (int i = 0; i < cnt; ++i) {
                    recursivePaths.append(path);
                }
            }
        }
        for (auto it = flatPathsToAdd.begin(); it != flatPathsToAdd.end(); ++it) {
            const QString &path = it.key();
            int cnt = it.value();
            if (cnt < 0) {
                requestsToRemove.append(JB::WatchRequest(path, false));
            } else {
                flatPaths.append(path);
            }
        }
        rootsNeedUpdate = fs->replaceWatchedRoots(requestsToRemove, recursivePaths, flatPaths);
        clearCachedPaths();
    }
    hasChangeEvent = false;
}

bool FileSystemNotifierPrivate::waitForPathsSet(int msecs) {
    if (!hasChangeEvent) {
        return true;
    }
    commitChange(); // if roots need update, signal must be sent to watcher and d

    if (!rootsNeedUpdate) {
        return true;
    }

    QElapsedTimer stopWatch;
    stopWatch.start();

    // Wait for sending roots command to fsnotifier
    while (!fs->fileWatcher()->isSendingRoots()) {
        if (stopWatch.elapsed() > msecs) {
            return false;
        }
    }
    // Wait for fsnotifier to feedback
    while (fs->fileWatcher()->isSettingRoots()) {
        if (stopWatch.elapsed() > msecs) {
            return false;
        }
    }
    return true;
}

void FileSystemNotifierPrivate::clearCachedPaths() {
    recursivePathsToAdd.clear();
    flatPathsToAdd.clear();
}

QMap<QString, int> FileSystemNotifierPrivate::recursivePaths() const {
    auto requests = fs->currentWatchedRoots();
    QMap<QString, int> res;
    for (auto it = requests.begin(); it != requests.end(); ++it) {
        const auto &request = it->first;
        if (!request.isSymlink() && request.isRecursive()) {
            res.insert(request.rootPath(), it->second);
        }
    }
    return res;
}

QMap<QString, int> FileSystemNotifierPrivate::flatPaths() const {
    auto requests = fs->currentWatchedRoots();
    QMap<QString, int> res;
    for (auto it = requests.begin(); it != requests.end(); ++it) {
        const auto &request = it->first;
        if (!request.isSymlink() && !request.isRecursive()) {
            res.insert(request.rootPath(), it->second);
        }
    }
    return res;
}

QPair<QMap<QString, int>, QMap<QString, int>> FileSystemNotifierPrivate::paths() const {
    auto requests = fs->currentWatchedRoots();
    QPair<QMap<QString, int>, QMap<QString, int>> res;
    for (auto it = requests.begin(); it != requests.end(); ++it) {
        const auto &request = it->first;
        if (!request.isSymlink()) {
            (request.isRecursive() ? res.first : res.second).insert(request.rootPath(), it->second);
        }
    }
    return res;
}

QStringList FileSystemNotifierPrivate::listPathToNativeSeparators(const QStringList &paths) {
    QStringList res;
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        res.append(QDir::toNativeSeparators(*it));
    }
    return res;
}

QStringList FileSystemNotifierPrivate::listPathFromNativeSeparators(const QStringList &paths) {
    QStringList res;
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        res.append(QDir::fromNativeSeparators(*it));
    }
    return res;
}
