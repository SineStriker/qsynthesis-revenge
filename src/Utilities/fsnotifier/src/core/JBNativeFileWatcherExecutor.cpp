#include "JBNativeFileWatcherExecutor.h"
#include "JBFileWatcherNotificationSink.h"
#include "JBNativeFileWatcher.h"

#include <QSet>

JBNativeFileWatcherExecutor::JBNativeFileWatcherExecutor(QObject *parent)
    : JBExecutorService(parent) {
    s_watchers.insert(this);
    jbDebug() << "[Watcher] Execute service" << s_watchers.size() << "init";

    m_taskThread = new QThread(this);

    m_watcher = new JBNativeFileWatcher();
    m_watcher->moveToThread(m_taskThread);

    connect(this, &Executor::initializeRequested, m_watcher, &JBNativeFileWatcher::initialize);
    connect(this, &Executor::disposeRequested, m_watcher, &JBNativeFileWatcher::dispose);
    connect(this, &Executor::setRootsRequested, m_watcher, &JBNativeFileWatcher::setWatchRoots);
}

JBNativeFileWatcherExecutor::~JBNativeFileWatcherExecutor() {
    if (m_taskThread->isRunning()) {
        quit();
    }
    delete m_watcher;

    jbDebug() << "[Watcher] Execute service" << s_watchers.size() << "quit";
    s_watchers.remove(this);
}

JBNativeFileWatcher *JBNativeFileWatcherExecutor::watcher() const {
    return m_watcher;
}

void JBNativeFileWatcherExecutor::start(JBFileWatcherNotificationSink *sink) {
    m_taskThread->start();

    emit initializeRequested(sink);
    while (!m_watcher->isActive()) {
        // Wait for initialized
    }
}

void JBNativeFileWatcherExecutor::quit() {
    emit disposeRequested();
    while (m_watcher->isActive()) {
        // Wait for disposed
    }

    m_taskThread->quit();
    m_taskThread->wait();
}

void JBNativeFileWatcherExecutor::setRoots(const QStringList &recursive, const QStringList &flat) {
    emit setRootsRequested(recursive, flat);
}

bool JBNativeFileWatcherExecutor::isOperational() const {
    return m_watcher->isOperational();
}

QSet<JBNativeFileWatcherExecutor *> JBNativeFileWatcherExecutor::s_watchers;

QList<JBNativeFileWatcherExecutor *> JBNativeFileWatcherExecutor::watchers() {
    return s_watchers.values();
}
