#include "QsFileManager.h"
#include "private/QsFileManager_p.h"

#include "Kernel/QsCoreConfig.h"
#include "Serialization/QJsonFormatter.h"

#include "QsSystem.h"

#include <QFileDialog>

Q_SINGLETON_DECLARE(QsFileManager)

static const char Slash = '/';

static const char FILE_NAME_RECENT_JSON[] = "recent.json";

QsFileManager::QsFileManager(QObject *parent) : QsFileManager(*new QsFileManagerPrivate(), parent) {
}

QsFileManager::~QsFileManager() {
}

bool QsFileManager::load() {
    Q_D(QsFileManager);
    d->load_helper(qAppConf->appDir(QsCoreConfig::AppData) + Slash + FILE_NAME_RECENT_JSON);
    return true;
}

bool QsFileManager::save() {
    Q_D(QsFileManager);
    d->save_helper(qAppConf->appDir(QsCoreConfig::AppData) + Slash + FILE_NAME_RECENT_JSON);
    return true;
}

int QsFileManager::registerFileType(const QString &key, int hint) {
    Q_D(QsFileManager);
    d->fileRegMax = (hint > d->fileRegMax) ? hint : (d->fileRegMax + 1);
    d->recentMap.insert(d->fileRegMax,
                        QsFileManagerPrivate::RecentDesc{key, QFileSet(QFileSet::File)});
    return d->fileRegMax;
}

int QsFileManager::registerDirType(const QString &key, int hint) {
    Q_D(QsFileManager);
    d->dirRegMax = (hint > d->dirRegMax) ? hint : (d->dirRegMax + 1);
    d->recentMap.insert(d->dirRegMax,
                        QsFileManagerPrivate::RecentDesc{key, QFileSet(QFileSet::Directory)});
    return d->fileRegMax;
}

void QsFileManager::commitRecent(int rType, int cType, const QString &filename) {
    Q_D(QsFileManager);
    QFileSet *fs;
    auto it = d->recentMap.find(rType);
    if (it == d->recentMap.end()) {
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
        default:
            fs->clear();
            break;
    }
    emit recentCommited(rType);
}

QStringList QsFileManager::fetchRecent(int rType) const {
    Q_D(const QsFileManager);
    auto it = d->recentMap.find(rType);
    if (it == d->recentMap.end()) {
        return {};
    }
    return it.value().set.valid();
}

QString QsFileManager::openFile(const QString &title, const QString &filter, const QString &flag,
                                QWidget *parent) {
    Q_D(QsFileManager);
    QString path = QFileDialog::getOpenFileName(parent, title, d->getLastOpenPath(flag), filter);
    if (!path.isEmpty()) {
        d->saveLastOpenDir(flag, path);
    }
    return path;
}

QStringList QsFileManager::openFiles(const QString &title, const QString &filter,
                                     const QString &flag, QWidget *parent) {
    Q_D(QsFileManager);
    QStringList paths =
        QFileDialog::getOpenFileNames(parent, title, d->getLastOpenPath(flag), filter);
    if (!paths.isEmpty()) {
        d->saveLastOpenDir(flag, paths.back());
    }
    return paths;
}

QString QsFileManager::openDir(const QString &title, const QString &flag, QWidget *parent) {
    Q_D(QsFileManager);
    QString path = QFileDialog::getExistingDirectory(parent, title, d->getLastOpenPath(flag));
    if (!path.isEmpty()) {
        d->saveLastOpenDir(flag, path, false);
    }
    return path;
}

QString QsFileManager::saveFile(const QString &title, const QString &filename,
                                const QString &filter, const QString &flag, QWidget *parent) {
    Q_D(QsFileManager);
    QFileInfo info(filename);
    if (info.isRelative() || !QsFs::isDirExist(info.absolutePath())) {
        info.setFile(d->getLastOpenPath(flag) + Slash + info.fileName());
    }
    QString path =
        QFileDialog::getSaveFileName(parent, title, info.absoluteFilePath(), filter, nullptr);
    if (!path.isEmpty()) {
        d->saveLastOpenDir(flag, path);
    }
    return path;
}

QsFileManager::QsFileManager(QsFileManagerPrivate &d, QObject *parent)
    : QsAbstractManager(d, parent) {
    construct();
    d.init();
}
