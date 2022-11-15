#include "JBLocalFileSystem.h"
#include "JBLocalFileSystemTimer.h"
#include "JBNativeFileWatcher.h"
#include "JBNativeFileWatcherExecutor.h"

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QTimerEvent>

JBLocalFileSystem::JBLocalFileSystem(QObject *parent)
    : QObject(parent), myWatcher(new JBFileWatcher()), myTimer(new JBLocalFileSystemTimer(this)) {
    Q_ASSERT(!self);
    self = this;

    jbDebug() << "[Local FS] Local File System init";

    createWatchers(1);

    watchThread = new QThread(this);
    myWatcher->moveToThread(watchThread);

    timerThread = new QThread(this);
    myTimer->moveToThread(timerThread);

    myWatchRootsManager = new JBWatchRootsManager(myWatcher.data(), this);

    connect(myWatcher.data(), &JBFileWatcher::failureOccured, this,
            [this](const QString &reason) { //
                emit failureOccured(reason);
            });

    connect(watchThread, &QThread::started, myWatcher.data(), &JBFileWatcher::start);
    connect(watchThread, &QThread::finished, myWatcher.data(), &JBFileWatcher::dispose);

    connect(timerThread, &QThread::started, myTimer.data(), &JBLocalFileSystemTimer::start);
    connect(timerThread, &QThread::finished, myTimer.data(), &JBLocalFileSystemTimer::stop);

    // Callback
    connect(myTimer.data(), &JBLocalFileSystemTimer::afterMarkDirtyCallback, this,
            &JBLocalFileSystem::handleAfterMarkDirtyCallback);

    // Stop timer before app quit
    connect(qApp, &QCoreApplication::aboutToQuit, this, &JBLocalFileSystem::dispose);
}

JBLocalFileSystem::~JBLocalFileSystem() {
    if (!disposed()) {
        dispose();
    }
    destroyWatchers();

    jbDebug() << "[Local FS] Local File System quit";

    self = nullptr;
}

void JBLocalFileSystem::start() {
    if (disposed()) {
        myWatchRootsManager->clear();

        watchThread->start();
        timerThread->start();
        while (!myTimer->isRunning()) {
        }
    }
}

void JBLocalFileSystem::dispose() {
    if (!disposed()) {
        timerThread->quit();
        while (myTimer->isRunning()) {
        }
        timerThread->wait();

        watchThread->quit();
        watchThread->wait();
    }
}

bool JBLocalFileSystem::disposed() const {
    return !timerThread->isRunning();
}

JBFileWatcher *JBLocalFileSystem::fileWatcher() const {
    return myWatcher.data();
}

bool JBLocalFileSystem::replaceWatchedRoots(const QList<WatchRequest> &watchRequestsToRemove,
                                            const QStringList &recursiveRootsToAdd,
                                            const QStringList &flatRootsToAdd) {
    if (disposed()) {
        return {};
    }
    return myWatchRootsManager->replaceWatchedRoots(watchRequestsToRemove, recursiveRootsToAdd,
                                                    flatRootsToAdd);
}

QList<QPair<JBLocalFileSystem::WatchRequest, int>> JBLocalFileSystem::currentWatchedRoots() const {
    if (disposed()) {
        return {};
    }
    return JBFileWatcherUtils::SetToList(myWatchRootsManager->currentWatchRequests());
}

void JBLocalFileSystem::markSuspiciousFilesDirty(const QStringList &paths) {
    Q_UNUSED(paths)
    storeRefreshStatusToFiles();
}

bool JBLocalFileSystem::storeRefreshStatusToFiles() {
    return myTimer->storeRefreshStatusToFiles();
}

void JBLocalFileSystem::handleAfterMarkDirtyCallback(const QStringList &paths,
                                                     const QStringList &flatDirs,
                                                     const QStringList &recursiveDirs) {
    paths.isEmpty() ? void() : emit pathsDirty(paths);
    flatDirs.isEmpty() ? void() : emit flatDirsDirty(flatDirs);
    recursiveDirs.isEmpty() ? void() : emit recursivePathsDirty(recursiveDirs);
}

JBLocalFileSystem *JBLocalFileSystem::self = nullptr;

JBLocalFileSystem *JBLocalFileSystem::instance() {
    if (!self) {
        jbDebug() << "[Local FS] Auto create JetBrains Local File System instance";
        new JBLocalFileSystem(qApp);
    }
    return self;
}

void JBLocalFileSystem::createWatchers(int n) {
    for (int i = 0; i < n; ++i) {
        auto e = new JBNativeFileWatcherExecutor();
        e->setParent(this);

        // e->setObjectName("com.intellij.vfs.local.pluggableFileWatcher");
    }
}

void JBLocalFileSystem::destroyWatchers() {
    auto es = JBNativeFileWatcherExecutor::watchers();
    for (auto it = es.begin(); it != es.end(); ++it) {
        delete *it;
    }
}
