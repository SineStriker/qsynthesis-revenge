#ifndef JBFILEWATCHERNOTIFICATIONSINK_H
#define JBFILEWATCHERNOTIFICATIONSINK_H

#include <QMap>
#include <QObject>

class JBPluggableFileWatcher;

/* File watcher notification slot */

class JBFileWatcherNotificationSink {
public:
    JBFileWatcherNotificationSink();
    virtual ~JBFileWatcherNotificationSink();

public:
    virtual void notifyManualWatchRoots(JBPluggableFileWatcher *watcher, const QStringList &roots);
    virtual void notifyMapping(const QList<QPair<QString, QString>> &mapping);
    virtual void notifyDirtyPath(const QString &path);
    virtual void notifyPathCreatedOrDeleted(const QString &path);
    virtual void notifyDirtyDirectory(const QString &path);
    virtual void notifyDirtyPathRecursive(const QString &path);
    virtual void notifyReset(const QString &path);
    virtual void notifyUserOnFailure(const QString &reason);
};

#endif // JBFILEWATCHERNOTIFICATIONSINK_H
