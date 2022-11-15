#ifndef JBNATIVEFILEWATCHEREXECUTOR_H
#define JBNATIVEFILEWATCHEREXECUTOR_H

#include "JBExecutorService.h"

class JBNativeFileWatcher;
class JBFileWatcherNotificationSink;

class JBNativeFileWatcherExecutor : public JBExecutorService {
    Q_OBJECT
public:
    explicit JBNativeFileWatcherExecutor(QObject *parent = nullptr);
    ~JBNativeFileWatcherExecutor();

    typedef JBNativeFileWatcherExecutor Executor;

public:
    JBNativeFileWatcher *watcher() const;

    void start(JBFileWatcherNotificationSink *sink);
    void quit();

    void setRoots(const QStringList &recursive, const QStringList &flat);

    bool isOperational() const;

private:
    JBNativeFileWatcher *m_watcher;

public:
    static QList<JBNativeFileWatcherExecutor *> watchers();

private:
    static QSet<JBNativeFileWatcherExecutor *> s_watchers;

signals:
    void initializeRequested(JBFileWatcherNotificationSink *sink);
    void disposeRequested();
    void setRootsRequested(const QStringList &recursive, const QStringList &flat);
};

#endif // JBNATIVEFILEWATCHEREXECUTOR_H
