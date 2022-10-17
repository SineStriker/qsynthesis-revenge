#include "JBFileWatcherAdvancedNSink.h"
#include "JBFileWatcher.h"
#include "JBNativeFileWatcher.h"
#include "JBNativeFileWatcherExecutor.h"
#include "JBPluggableFileWatcher.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>

JBFileWatcherAdvancedNSink::JBFileWatcherAdvancedNSink(JBFileWatcher *watcher)
    : myWatcher(watcher), myLock(new QMutex()) {
    Q_ASSERT(watcher);
}

JBFileWatcherAdvancedNSink::~JBFileWatcherAdvancedNSink() {
}

JBFileWatcherAdvancedNSink::DirtyPaths JBFileWatcherAdvancedNSink::getDirtyPaths() {
    QMutexLocker locker(myLock.data());

    DirtyPaths dirtyPaths;
    if (!myDirtyPaths.isEmpty()) {
        dirtyPaths = myDirtyPaths;
        myDirtyPaths.clear();
    }

    const auto &watchers = JBNativeFileWatcherExecutor::watchers();
    for (auto it = watchers.begin(); it != watchers.end(); ++it) {
        auto watcher = *it;
        watcher->watcher()->resetChangedPaths();
    }

    return dirtyPaths;
}

void JBFileWatcherAdvancedNSink::notifyManualWatchRoots(JBPluggableFileWatcher *watcher,
                                                        const QStringList &roots) {
    registerManualWatchRoots(watcher, QSet<QString>(roots.begin(), roots.end()));
}

void JBFileWatcherAdvancedNSink::notifyMapping(const QList<QPair<QString, QString>> &mapping) {
    if (!mapping.isEmpty()) {
        myWatcher->myPathMap.addMapping(mapping);
    }
    notifyOnEvent(OTHER);
}

void JBFileWatcherAdvancedNSink::notifyDirtyPath(const QString &path) {
    auto paths = myWatcher->myPathMap.mapToOriginalWatchRoots(path, true);
    if (!paths.isEmpty()) {
        QMutexLocker locker(myLock.data());
        for (auto it = paths.begin(); it != paths.end(); ++it) {
            myDirtyPaths.addDirtyPath(*it);
        }
    }
    notifyOnEvent(path);
}

void JBFileWatcherAdvancedNSink::notifyPathCreatedOrDeleted(const QString &path) {
    auto paths = myWatcher->myPathMap.mapToOriginalWatchRoots(path, true);
    auto parentPaths = myWatcher->myPathMap.mapToUpperWatchRoots(path);
    if (!paths.isEmpty() || !parentPaths.isEmpty()) {
        QMutexLocker locker(myLock.data());
        for (auto it = paths.begin(); it != paths.end(); ++it) {
            const QString &p = *it;
            myDirtyPaths.addDirtyPathRecursive(p);
            QString parentPath = JBFileWatcherUtils::PathUtil::getParentPath(p);
            if (!parentPath.isEmpty()) {
                myDirtyPaths.addDirtyPath(parentPath);
            }
        }
        // Potential recursively changed parent paths
        for (auto it = parentPaths.begin(); it != parentPaths.end(); ++it) {
            const QString &p = *it;
            myDirtyPaths.addDirtyPathRecursive(p);
        }
    }
    notifyOnEvent(path);
}

void JBFileWatcherAdvancedNSink::notifyDirtyDirectory(const QString &path) {
    auto paths = myWatcher->myPathMap.mapToOriginalWatchRoots(path, false);
    if (!paths.isEmpty()) {
        QMutexLocker locker(myLock.data());
        myDirtyPaths.addDirtyDirs(paths);
    }
    notifyOnEvent(path);
}

void JBFileWatcherAdvancedNSink::notifyDirtyPathRecursive(const QString &path) {
    auto paths = myWatcher->myPathMap.mapToOriginalWatchRoots(path, false);
    if (!paths.isEmpty()) {
        QMutexLocker locker(myLock.data());
        for (auto it = paths.begin(); it != paths.end(); ++it) {
            myDirtyPaths.addDirtyPathRecursive(*it);
        }
    }
    notifyOnEvent(path);
}

void JBFileWatcherAdvancedNSink::notifyReset(const QString &path) {
    if (!path.isEmpty()) {
        QMutexLocker locker(myLock.data());
        myDirtyPaths.addDirtyPathRecursive(path);
    } else {
        QFileInfoList roots = QDir::drives();
        QMutexLocker locker(myLock.data());
        for (auto it = roots.begin(); it != roots.end(); ++it) {
            auto root = *it;
            myDirtyPaths.addDirtyPathRecursive(QDir::toNativeSeparators(root.absoluteFilePath()));
        }
    }
    notifyOnEvent(RESET);
}

void JBFileWatcherAdvancedNSink::notifyUserOnFailure(const QString &reason) {
    myWatcher->notifyOnFailure(reason);
}

void JBFileWatcherAdvancedNSink::registerManualWatchRoots(QObject *key,
                                                          const QSet<QString> &roots) {
    myWatcher->registerManualWatchRoots(key, roots);
    notifyOnEvent(OTHER);
}

void JBFileWatcherAdvancedNSink::notifyOnEvent(const QString &path) {
    myWatcher->notifyOnEvent(path);
}
