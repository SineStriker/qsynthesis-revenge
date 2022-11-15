#include "FileSystemNotifier.h"
#include "private/FileSystemNotifier_p.h"
#include "core/JBFileWatchNamespace.h"

using namespace JB::Utils;

#include <QCoreApplication>

FileSystemNotifier::FileSystemNotifier(QObject *parent)
    : FileSystemNotifier(*new FileSystemNotifierPrivate(this), parent) {
}

FileSystemNotifier::~FileSystemNotifier() {
}

FileSystemNotifier::FileSystemNotifier(FileSystemNotifierPrivate &d, QObject *parent)
    : QObject(parent), d(&d) {

    Q_ASSERT(!self);
    self = this;

    this->d->init();
}

void FileSystemNotifier::startWatch() {
    d->fs->start();
}

void FileSystemNotifier::stopWatch() {
    d->fs->dispose();
}

bool FileSystemNotifier::isWatching() const {
    return !d->fs->disposed();
}

void FileSystemNotifier::addRecursivePaths(const QStringList &paths) {
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        d->changeRecursivePathsToAdd(*it, 1);
    }
    d->postChange();
}

void FileSystemNotifier::addFlatPaths(const QStringList &paths) {
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        d->changeFlatPathsToAdd(*it, 1);
    }
    d->postChange();
}

void FileSystemNotifier::removeRecursivePaths(const QStringList &paths) {
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        d->changeRecursivePathsToAdd(*it, -1);
    }
    d->postChange();
}

void FileSystemNotifier::removeFlatPaths(const QStringList &paths) {
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        d->changeFlatPathsToAdd(*it, -1);
    }
    d->postChange();
}

void FileSystemNotifier::removeAllRecursivePaths() {
    const auto &paths = d->recursivePaths();
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        d->changeRecursivePathsToAdd(it.key(), -it.value());
    }
    d->postChange();
}

void FileSystemNotifier::removeAllFlatPaths() {
    const auto &paths = d->flatPaths();
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        d->changeRecursivePathsToAdd(it.key(), -it.value());
    }
    d->postChange();
}

void FileSystemNotifier::removeAllPaths() {
    const auto &pathsPair = d->paths();
    d->clearCachedPaths();

    const auto &recPairs = pathsPair.first;
    for (auto it = recPairs.begin(); it != recPairs.end(); ++it) {
        d->changeRecursivePathsToAdd(it.key(), -it.value());
    }

    const auto &flatPairs = pathsPair.second;
    for (auto it = flatPairs.begin(); it != flatPairs.end(); ++it) {
        d->changeFlatPathsToAdd(it.key(), -it.value());
    }
    d->postChange();
}

QList<QPair<QString, int>> FileSystemNotifier::recursivePaths() const {
    auto paths = d->recursivePaths();
    QList<QPair<QString, int>> res;
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        res.append(qMakePair(it.key(), it.value()));
    }
    return res;
}

QList<QPair<QString, int>> FileSystemNotifier::flatPaths() const {
    auto paths = d->flatPaths();
    QList<QPair<QString, int>> res;
    for (auto it = paths.begin(); it != paths.end(); ++it) {
        res.append(qMakePair(it.key(), it.value()));
    }
    return res;
}

bool FileSystemNotifier::waitForPathsSet(int msecs) {
    return d->waitForPathsSet(msecs);
}

bool FileSystemNotifier::separatorSystemDependent() const {
    return d->separatorSystemDependent;
}

void FileSystemNotifier::setSeparatorSystemDependent(bool val) {
    d->separatorSystemDependent = val;
}

bool FileSystemNotifier::event(QEvent *event) {
    if (event->type() == QEvent::Timer) {
        QTimerEvent *e = static_cast<QTimerEvent *>(event);
        int id = -e->timerId();
        if (id > 0) {
            if (d->hasChangeEvent) {
                d->commitChange();
                return true;
            }
            return false;
        }
    }
    return QObject::event(event);
}

FileSystemNotifier *FileSystemNotifier::self = nullptr;

FileSystemNotifier *FileSystemNotifier::instance() {
    if (!self) {
        new FileSystemNotifier(qApp);
    }
    return self;
}

QString FileSystemNotifier::ExecutableFilePath() {
    return JBNativeFileWatcher::FSNotifierExecutable();
}

void FileSystemNotifier::setExecutableFilePath(const QString &path) {
    JBNativeFileWatcher::setFsNotifierExecutablePath(path);
}
