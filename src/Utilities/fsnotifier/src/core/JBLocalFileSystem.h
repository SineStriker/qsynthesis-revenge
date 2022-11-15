#ifndef JBLOCALFILESYSTEM_H
#define JBLOCALFILESYSTEM_H

#include "JBFileWatcher.h"
#include "JBWatchRootsManager.h"

#include <QThread>

#define JBFS JBLocalFileSystem::instance()

class JBLocalFileSystemTimer;

class JBLocalFileSystem : public QObject {
    Q_OBJECT
public:
    explicit JBLocalFileSystem(QObject *parent = nullptr);
    ~JBLocalFileSystem();

    typedef JBFileWatchRequest WatchRequest;

public:
    void start();
    void dispose();

    bool disposed() const;

    JBFileWatcher *fileWatcher() const;
    bool replaceWatchedRoots(const QList<WatchRequest> &watchRequestsToRemove,
                             const QStringList &recursiveRootsToAdd,
                             const QStringList &flatRootsToAdd);
    QList<QPair<WatchRequest, int>> currentWatchedRoots() const;

    void markSuspiciousFilesDirty(const QStringList &paths);

private:
    QScopedPointer<JBFileWatcher> myWatcher;
    JBWatchRootsManager *myWatchRootsManager;

    QThread *timerThread;
    QThread *watchThread;
    QList<QThread *> processThreads;

    QScopedPointer<JBLocalFileSystemTimer> myTimer;

    bool storeRefreshStatusToFiles();

    // Slot
    void handleAfterMarkDirtyCallback(const QStringList &paths, const QStringList &flatDirs,
                                      const QStringList &recursiveDirs);

    // Singleton
public:
    static JBLocalFileSystem *instance();

private:
    static JBLocalFileSystem *self;

    void createWatchers(int n);
    void destroyWatchers();

signals:
    void pathsDirty(const QStringList &paths);
    void flatDirsDirty(const QStringList &paths);
    void recursivePathsDirty(const QStringList &paths);
    void failureOccured(const QString &reason);
};

#endif // JBLOCALFILESYSTEM_H
