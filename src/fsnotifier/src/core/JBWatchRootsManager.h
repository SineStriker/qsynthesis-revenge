#ifndef JBWATCHROOTSMANAGER_H
#define JBWATCHROOTSMANAGER_H

#include <QMutex>
#include <QObject>

#include "JBCanonicalPathMap.h"
#include "JBFileWatchRequest.h"
#include "JBFileWatcherAdvancedNSink.h"
#include "JBFileWatcherNotificationSink.h"
#include "JBNativeFileWatcher.h"

// Symbolic link part is not implemented.

class JBWatchRootsManager : public QObject {
    Q_OBJECT
public:
    explicit JBWatchRootsManager(JBFileWatcher *watcher, QObject *parent = nullptr);
    ~JBWatchRootsManager();

    typedef JBFileWatchRequest WatchRequest;

public:
    bool replaceWatchedRoots(const QList<WatchRequest> &requestsToRemove,
                             const QStringList &recursiveRootsToAdd,
                             const QStringList &flatRootsToAdd);

    QSet<QPair<WatchRequest, int>> currentWatchRequests() const;

    void clear();

    void updateSymlink(int fileId, const QString &linkPath, const QString &linkTarget);
    void removeSymlink(int fileId);

private:
    JBFileWatcher *myFileWatcher;

    JBNavigableFileMap<QList<WatchRequest>> myRecursiveWatchRoots;
    JBNavigableFileMap<QList<WatchRequest>> myFlatWatchRoots;
    JBNavigableFileSet myOptimizedRecursiveWatchRoots;
    JBNavigableFileMap<WatchRequest> mySymlinksByPath;
    QMap<int, WatchRequest> mySymlinksById;
    JBNavigableSet<QPair<QString, QString>> myPathMappings;
    bool myWatcherRequiresUpdate;

    QScopedPointer<QMutex> myLock;

    void updateFileWatcher();

    JBCanonicalPathMap createCanonicalPathMap(const QSet<QString> &flatWatchRoots,
                                              const QSet<QString> &optimizedRecursiveWatchRoots,
                                              const QList<QPair<QString, QString>> &pathMappings,
                                              bool convertToForwardSlashes);

    void updateWatchRoots(QList<QString> rootsToAdd, QList<WatchRequest> requestsToRemove,
                          QList<WatchRequest> &result,
                          JBNavigableFileMap<QList<WatchRequest>> &roots, bool recursiveWatchRoots);

    QString prepareWatchRoot(QString root);

    void removeWatchRequest(WatchRequest request);

    void addWatchSymlinkRequests(const QList<WatchRequest> &watchSymlinkRequestsToAdd);
    void addWatchSymlinkRequest(WatchRequest request);

    void removeWatchSymlinkRequests(const QList<WatchRequest> &watchSymlinkRequestsToRemove);
    void removeWatchSymlinkRequest(WatchRequest request);

    void collectSymlinkRequests(WatchRequest newRequest,
                                QList<WatchRequest> &watchSymlinkRequestsToAdd);

signals:
    void setRootsRequested(const JBCanonicalPathMap &map);
};

#endif // JBWATCHROOTSMANAGER_H
