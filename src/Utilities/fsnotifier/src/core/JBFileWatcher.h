#ifndef JBFILEWATCHER_H
#define JBFILEWATCHER_H

#include "JBCanonicalPathMap.h"
#include "JBFileWatcherAdvancedNSink.h"

class JBFileWatcher : public QObject {
    Q_OBJECT
public:
    explicit JBFileWatcher(QObject *parent = nullptr);
    ~JBFileWatcher();

    friend class JBFileWatcherAdvancedNSink;

private:
    void init();

public:
    // Thread unsafe
    void start();

    // Thread unsafe
    void dispose();

public:
    bool isOperational() const;
    bool isSettingRoots() const;

    bool isActive() const;
    bool isSendingRoots() const;

    JBFileWatcherDirtyPaths getDirtyPaths() const;

    // Thread unsafe
    QStringList getManualWatchRoots() const;

    // Thread unsafe
    QStringList mapToAllSymlinks(const QString &reportedPath) const;

    // Thread unsafe
    bool belongsToWatchRoots(const QString &reportedPath, bool isFile) const;

    // Thread unsafe
    void setWatchRoots(const JBCanonicalPathMap &pathMap);

    // Thread unsafe
    void notifyOnFailure(const QString &reason);

private:
    JBCanonicalPathMap myPathMap;
    QMap<QObject *, QSet<QString>> myManualWatchRoots;

    QScopedPointer<JBFileWatcherAdvancedNSink> myNotificationSink;

    void clear();

    void registerManualWatchRoots(QObject *key, const QSet<QString> &roots);
    void notifyOnEvent(const QString &path);

signals:
    void failureOccured(const QString &reason);
};

#endif // JBFILEWATCHER_H
