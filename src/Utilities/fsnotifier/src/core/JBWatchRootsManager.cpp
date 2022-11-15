#include "JBWatchRootsManager.h"
#include "JBFileWatcher.h"
#include "JBFileWatcherUtils.h"

using namespace JBFileWatcherUtils;

#include <QDebug>

JBWatchRootsManager::JBWatchRootsManager(JBFileWatcher *watcher, QObject *parent)
    : QObject(parent), myFileWatcher(watcher), myLock(new QMutex()) {
    myWatcherRequiresUpdate = false;
    connect(this, &JBWatchRootsManager::setRootsRequested, myFileWatcher,
            &JBFileWatcher::setWatchRoots);
}

JBWatchRootsManager::~JBWatchRootsManager() {
}

bool JBWatchRootsManager::replaceWatchedRoots(const QList<WatchRequest> &requestsToRemove,
                                              const QStringList &recursiveRootsToAdd,
                                              const QStringList &flatRootsToAdd) {
    QMutexLocker locker(myLock.data());

    QList<WatchRequest> recursiveRequestsToRemove, flatRequestsToRemove;
    for (auto it = requestsToRemove.begin(); it != requestsToRemove.end(); ++it) {
        const auto &req = *it;
        (req.isRecursive() ? recursiveRequestsToRemove : flatRequestsToRemove).append(req);
    };

    QList<WatchRequest> result;

    updateWatchRoots(recursiveRootsToAdd, recursiveRequestsToRemove, result,
                     myRecursiveWatchRoots, true);
    updateWatchRoots(flatRootsToAdd, flatRequestsToRemove, result, myFlatWatchRoots,
                     false);

    bool updated = myWatcherRequiresUpdate;
    if (myWatcherRequiresUpdate) {
        updateFileWatcher();
    }

    Q_UNUSED(result)
    return updated;
}

QSet<QPair<JBWatchRootsManager::WatchRequest, int>>
    JBWatchRootsManager::currentWatchRequests() const {
    QMutexLocker locker(myLock.data());
    QSet<QPair<JBWatchRootsManager::WatchRequest, int>> res;
    for (auto it = myRecursiveWatchRoots.begin(); it != myRecursiveWatchRoots.end(); ++it) {
        res.insert(qMakePair(it->second.front(), it->second.size()));
    }
    for (auto it = myFlatWatchRoots.begin(); it != myFlatWatchRoots.end(); ++it) {
        res.insert(qMakePair(it->second.front(), it->second.size()));
    }
    return res;
}

void JBWatchRootsManager::clear() {
    QMutexLocker locker(myLock.data());
    myRecursiveWatchRoots.clear();
    myOptimizedRecursiveWatchRoots.clear();
    myFlatWatchRoots.clear();
    myPathMappings.clear();
    mySymlinksById.clear();
}

void JBWatchRootsManager::updateSymlink(int fileId, const QString &linkPath,
                                        const QString &linkTarget) {
    QMutexLocker locker(myLock.data());

    auto it = mySymlinksById.find(fileId);
    if (it != mySymlinksById.end()) {
        const auto &request = it.value();
        auto data = request.symlinkData();
        if (FileUtil::pathsEqual(data.path(), linkPath) &&
            FileUtil::pathsEqual(data.target(), linkTarget)) {
            // Avoid costly removal and re-addition of the request in case of no-op update
            return;
        }
        mySymlinksById.remove(fileId);
        mySymlinksByPath.remove(data.path());

        removeWatchSymlinkRequest(request);
    }

    JBSymlinkData data(fileId, linkPath, linkTarget);
    auto it2 = mySymlinksByPath.find(linkPath);
    if (it2 != mySymlinksByPath.end()) {
        jbWarning() << "[Warning] Path conflict. Existing symlink: " +
                           it2->second.symlinkData().toString() +
                           " vs. new symlink: " + data.toString();
        return;
    }

    WatchRequest request(data, true);
    mySymlinksByPath.insert(data.path(), request);
    mySymlinksById.insert(data.id(), request);

    if (data.hasValidTarget() &&
        WatchRootsUtil::isCoveredRecursively(myOptimizedRecursiveWatchRoots, data.path())) {
        addWatchSymlinkRequest(request);
    }
}

void JBWatchRootsManager::removeSymlink(int fileId) {
    QMutexLocker locker(myLock.data());

    auto it = mySymlinksById.find(fileId);
    if (it != mySymlinksById.end()) {
        mySymlinksById.erase(it);
        removeWatchSymlinkRequest(it.value());
    }
}

void JBWatchRootsManager::updateFileWatcher() {
    if (!myWatcherRequiresUpdate) {
        return;
    }
    myWatcherRequiresUpdate = false;

    emit setRootsRequested(createCanonicalPathMap(
        ListToSet(myFlatWatchRoots.toQMap().keys()), myOptimizedRecursiveWatchRoots.toQSet(),
        myPathMappings.toQList(), QDir::separator() == '\\'));
}

JBCanonicalPathMap JBWatchRootsManager::createCanonicalPathMap(
    const QSet<QString> &flatWatchRoots, const QSet<QString> &optimizedRecursiveWatchRoots,
    const QList<QPair<QString, QString>> &pathMappings, bool convertToForwardSlashes) {

    JBNavigableFileSet optimizedRecursiveWatchRootsCopy;
    QList<QPair<QString, QString>> initialMappings;

    // Ensure paths are system dependent
    if (!convertToForwardSlashes) {
        optimizedRecursiveWatchRootsCopy = optimizedRecursiveWatchRoots;
        initialMappings = pathMappings;
    } else {
        for (QString recursiveWatchRoot : optimizedRecursiveWatchRoots) {
            optimizedRecursiveWatchRootsCopy.insert(recursiveWatchRoot.replace('/', '\\'));
        }
        for (auto it = pathMappings.begin(); it != pathMappings.end(); ++it) {
            auto mapping = *it;
            initialMappings.append(
                qMakePair(mapping.first.replace('/', '\\'), mapping.second.replace('/', '\\')));
        }
    }

    JBNavigableFileSet optimizedFlatWatchRoots = WatchRootsUtil::optimizeFlatRoots(
        flatWatchRoots, optimizedRecursiveWatchRootsCopy, convertToForwardSlashes);

    return JBCanonicalPathMap(optimizedRecursiveWatchRootsCopy, optimizedFlatWatchRoots,
                              initialMappings);
}

void JBWatchRootsManager::updateWatchRoots(QList<QString> rootsToAdd,
                                           QList<WatchRequest> requestsToRemove,
                                           QList<WatchRequest> &result,
                                           JBNavigableFileMap<QList<WatchRequest>> &roots,
                                           bool recursiveWatchRoots) {
    QList<WatchRequest> watchSymlinkRequestsToAdd;
    for (const QString &root : rootsToAdd) {
        QString watchRoot = prepareWatchRoot(root);
        if (watchRoot.isEmpty()) {
            continue;
        }

        // add to member roots
        auto it = roots.computeIfAbsent(watchRoot, {});
        QList<WatchRequest> &requests = it->second; // executory requests
        bool foundSameRequest = false;
        if (!requestsToRemove.isEmpty()) {
            for (const WatchRequest &currentRequest : requests) {
                if (requestsToRemove.removeOne(currentRequest)) { // found in executory requests
                    foundSameRequest = true;
                    result.append(currentRequest); // not remove
                }
            }
        }

        // if found,
        if (!foundSameRequest) {
            WatchRequest newRequest(watchRoot, recursiveWatchRoots);
            requests.append(newRequest);
            result.append(newRequest);
            if (recursiveWatchRoots) {
                collectSymlinkRequests(newRequest, watchSymlinkRequestsToAdd);
            }
            if (requests.size() == 1 &&
                !WatchRootsUtil::isCoveredRecursively(myOptimizedRecursiveWatchRoots, watchRoot)) {
                myWatcherRequiresUpdate = true;
                if (recursiveWatchRoots) {
                    WatchRootsUtil::insertRecursivePath(myOptimizedRecursiveWatchRoots, watchRoot);
                }
            }
        }
    }

    QList<WatchRequest> watchSymlinkRequestsToRemove;
    for (const WatchRequest &request : requestsToRemove) {
        removeWatchRequest(request);
        if (recursiveWatchRoots) {
            collectSymlinkRequests(request, watchSymlinkRequestsToRemove);
        }
    }

    if (recursiveWatchRoots) {
        addWatchSymlinkRequests(watchSymlinkRequestsToAdd);
        removeWatchSymlinkRequests(watchSymlinkRequestsToRemove);
    }
}

QString JBWatchRootsManager::prepareWatchRoot(QString root) {
    int index = root.indexOf("!/");
    if (index >= 0) {
        root = root.mid(0, index);
    }
    if (QDir::isRelativePath(root)) {
        jbWarning() << "[Warning] Watch roots should be absolute";
        return "";
    }
    return QDir::toNativeSeparators(root);
}

void JBWatchRootsManager::removeWatchRequest(JBWatchRootsManager::WatchRequest request) {
    QString watchRoot = request.rootPath();
    auto &roots = request.isRecursive() ? myRecursiveWatchRoots : myFlatWatchRoots;

    auto it = roots.find(watchRoot);
    if (it == roots.end()) {
        return;
    }
    QList<WatchRequest> &requests = it->second;
    requests.removeOne(request);
    if (requests.isEmpty()) {
        roots.erase(it);
        if (request.isRecursive()) {
            if (WatchRootsUtil::removeRecursivePath(myOptimizedRecursiveWatchRoots,
                                                    myRecursiveWatchRoots, watchRoot)) {
                myWatcherRequiresUpdate = true;
            }
        } else if (!WatchRootsUtil::isCoveredRecursively(myOptimizedRecursiveWatchRoots,
                                                         watchRoot)) {
            myWatcherRequiresUpdate = true;
        }
    }
}

void JBWatchRootsManager::addWatchSymlinkRequests(
    const QList<JBWatchRootsManager::WatchRequest> &watchSymlinkRequestsToAdd) {
    for (const WatchRequest &request : watchSymlinkRequestsToAdd) {
        if (!request.rootPath().isEmpty() && !request.isRegistered()) {
            addWatchSymlinkRequest(request);
        }
    }
}

void JBWatchRootsManager::addWatchSymlinkRequest(JBWatchRootsManager::WatchRequest request) {
    QString watchRoot = request.rootPath();
    auto &roots = request.isRecursive() ? myRecursiveWatchRoots : myFlatWatchRoots;

    auto it = roots.find(watchRoot);
    if (it == roots.end()) {
        it = roots.insert(watchRoot, {}).first;
    }
    QList<WatchRequest> &requests = it->second;

    requests.append(request);
    if (requests.size() == 1 &&
        !WatchRootsUtil::isCoveredRecursively(myOptimizedRecursiveWatchRoots, watchRoot)) {
        if (request.isRecursive()) {
            WatchRootsUtil::insertRecursivePath(myOptimizedRecursiveWatchRoots, watchRoot);
        }
    }
    if (request.setRegistered(true)) {
        myWatcherRequiresUpdate = true;
        myPathMappings.insert(qMakePair(watchRoot, request.orgPath()));
    }
}

void JBWatchRootsManager::removeWatchSymlinkRequests(
    const QList<JBWatchRootsManager::WatchRequest> &watchSymlinkRequestsToRemove) {
    for (const auto &request : watchSymlinkRequestsToRemove) {
        bool remove = true;
        WatchRootsUtil::forEachPathSegment(
            request.orgPath(), '/', [&](const QString &path) -> bool {
                auto it = myRecursiveWatchRoots.find(path);
                if (it != myRecursiveWatchRoots.end()) {
                    const auto &requests = it->second;
                    for (auto it2 = requests.begin(); it2 != requests.end(); ++it2) {
                        if (!it2->isSymlink()) {
                            remove = false;
                            return false;
                        }
                    }
                }
                return true;
            });
        if (remove) {
            removeWatchSymlinkRequest(request);
        }
    }
}

void JBWatchRootsManager::removeWatchSymlinkRequest(WatchRequest request) {
    if (!request.isRegistered()) {
        return;
    }
    removeWatchRequest(request);
    if (request.setRegistered(false)) {
        myPathMappings.remove({request.rootPath(), request.orgPath()});
        myWatcherRequiresUpdate = true;
    }
}

void JBWatchRootsManager::collectSymlinkRequests(
    JBWatchRootsManager::WatchRequest newRequest,
    QList<JBWatchRootsManager::WatchRequest> &watchSymlinkRequestsToAdd) {

    const QString &prefix = newRequest.rootPath();
    for (auto it = mySymlinksByPath.higher(prefix); it != mySymlinksByPath.end(); ++it) {
        if (OSAgnosticPathUtil::startsWith(it->first, prefix)) {
            if (it->second.symlinkData().hasValidTarget()) {
                watchSymlinkRequestsToAdd.append(it->second);
            }
        } else {
            break;
        }
    }
}
