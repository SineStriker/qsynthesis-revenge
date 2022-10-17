#ifndef JBFILEWATCHERADVANCEDNSINK_H
#define JBFILEWATCHERADVANCEDNSINK_H

#include "JBFileWatcherDirtyPaths.h"
#include "JBFileWatcherNotificationSink.h"

#include <QMutex>

class JBFileWatcher;

class JBFileWatcherAdvancedNSink : public JBFileWatcherNotificationSink {
public:
    JBFileWatcherAdvancedNSink(JBFileWatcher *watcher);
    ~JBFileWatcherAdvancedNSink();

    typedef JBFileWatcherDirtyPaths DirtyPaths;

public:
    DirtyPaths getDirtyPaths();

    const QString RESET = "(reset)";
    const QString OTHER = "(other)";

public:
    void notifyManualWatchRoots(JBPluggableFileWatcher *watcher, const QStringList &roots) override;
    void notifyMapping(const QList<QPair<QString, QString>> &mapping) override;
    void notifyDirtyPath(const QString &path) override;
    void notifyPathCreatedOrDeleted(const QString &path) override;
    void notifyDirtyDirectory(const QString &path) override;
    void notifyDirtyPathRecursive(const QString &path) override;
    void notifyReset(const QString &path) override;
    void notifyUserOnFailure(const QString &reason) override;

private:
    void registerManualWatchRoots(QObject *key, const QSet<QString> &roots);
    void notifyOnEvent(const QString &path);

private:
    JBFileWatcher *myWatcher;

    DirtyPaths myDirtyPaths;

    QScopedPointer<QMutex> myLock;
};

#endif // JBFILEWATCHERADVANCEDNSINK_H
