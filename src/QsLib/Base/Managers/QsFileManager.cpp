#include "QsFileManager.h"
#include "private/QsFileManager_p.h"

#include "Kernel/QsCoreConfig.h"
#include "Serialization/QJsonFormatter.h"

#include "QsSystem.h"

Q_SINGLETON_DECLARE(QsFileManager)

static const char Slash = '/';

static const char FILE_NAME_RECENT_JSON[] = "recent.json";

QsFileManager::QsFileManager(QObject *parent) : QsFileManager(*new QsFileManagerPrivate(), parent) {
}

QsFileManager::~QsFileManager() {
}

void QsFileManager::loadImpl() {
    Q_D(QsFileManager);
    d->load_helper(qAppConf->appDir(QsCoreConfig::AppData) + Slash + FILE_NAME_RECENT_JSON);
}

void QsFileManager::saveImpl() {
    Q_D(QsFileManager);
    d->save_helper(qAppConf->appDir(QsCoreConfig::AppData) + Slash + FILE_NAME_RECENT_JSON);
}

int QsFileManager::registerFileType(const QString &key, int hint) {
    Q_D(QsFileManager);
    d->fileRegMax = (hint > d->fileRegMax) ? hint : (d->fileRegMax + 1);
    d->recentFileMap.insert(d->fileRegMax, QsFileManagerPrivate::RecentDesc{key, QFileSet(QFileSet::File)});
    return d->fileRegMax;
}

int QsFileManager::registerDirType(const QString &key, int hint) {
    Q_D(QsFileManager);
    d->dirRegMax = (hint > d->dirRegMax) ? hint : (d->dirRegMax + 1);
    d->recentFileMap.insert(d->dirRegMax, QsFileManagerPrivate::RecentDesc{key, QFileSet(QFileSet::Directory)});
    return d->fileRegMax;
}

void QsFileManager::commitRecent(int fType, int cType, const QString &filename) {
    Q_D(QsFileManager);
    QFileSet *fs;
    auto it = d->recentFileMap.find(fType);
    if (it == d->recentFileMap.end()) {
        return;
    }
    fs = &it.value().set;
    switch (cType) {
        case Push:
            fs->push(filename);
            break;
        case Unshift:
            fs->unshift(filename);
            break;
        case Advance:
            fs->advance(filename);
            break;
        case Remove:
            fs->remove(filename);
            break;
        case Clear:
            fs->clear();
            break;
        default:
            break;
    }
    emit recentCommited(fType);
}

QStringList QsFileManager::fetchRecent(int fType) const {
    Q_D(const QsFileManager);
    auto it = d->recentFileMap.find(fType);
    if (it == d->recentFileMap.end()) {
        return {};
    }
    return it.value().set.valid();
}

QString QsFileManager::openFile(const QString &title, const QString &filter, const QString &flag, QObject *parent) {
    return {};
}

QStringList QsFileManager::openFiles(const QString &title, const QString &filter, const QString &flag,
                                     QObject *parent) {
    return {};
}

QString QsFileManager::openDir(const QString &title, const QString &flag, QObject *parent) {
    return {};
}

QString QsFileManager::saveFile(const QString &title, const QString &filename, const QString &filter,
                                const QString &flag, QObject *parent) {
    return {};
}

QsFileManager::QsFileManager(QsFileManagerPrivate &d, QObject *parent) : QsAbstractManager(d, parent) {
    construct();
    d.init();
}
