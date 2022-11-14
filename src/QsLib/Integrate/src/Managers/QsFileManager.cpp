#include "QsFileManager.h"
#include "private/QsFileManager_p.h"

#include "Kernel/QsDistConfig.h"
#include "Serialization/QJsonFormatter.h"

#include "SystemHelper.h"

#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardPaths>

Q_SINGLETON_DECLARE(QsFileManager)

static const char Slash = '/';

static const char FILE_NAME_RECENT_JSON[] = "recent.json";
static const char KEY_NAME_RECENT_FILES[] = "files";
static const char KEY_NAME_RECENT_DIRS[] = "dirs";

QsFileManager::QsFileManager(QObject *parent) : QsFileManager(*new QsFileManagerPrivate(), parent) {
}

QsFileManager::~QsFileManager() {
}

bool QsFileManager::load() {
    Q_D(QsFileManager);
    QFile file(qAppConf->appDir(QsDistConfig::AppData) + Slash + FILE_NAME_RECENT_JSON);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data(file.readAll());
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonFormatter f;
            QStringList files, dirs;
            bool parsed = f.parse(doc.object(), f.OT({{KEY_NAME_RECENT_FILES, f.SL(&files)},
                                                      {KEY_NAME_RECENT_DIRS, f.SL(&dirs)}}));
            if (parsed) {
                d->projects.set(files);
                d->folders.set(dirs);
            }
        }
        file.close();
    }
    return true;
}

bool QsFileManager::save() {
    Q_D(QsFileManager);
    QFile file(qAppConf->appDir(QsDistConfig::AppData) + Slash + FILE_NAME_RECENT_JSON);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QJsonDocument doc;
        QJsonObject obj{
            {KEY_NAME_RECENT_FILES, QJsonArray::fromStringList(d->projects.valid())},
            {KEY_NAME_RECENT_DIRS, QJsonArray::fromStringList(d->folders.valid())},
        };
        doc.setObject(obj);
        file.write(doc.toJson());
        file.close();
    }
    return true;
}

void QsFileManager::commitRecent(QsFileManager::RecentType rType, QsFileManager::ChangeType cType,
                               const QString &filename) {
    Q_D(QsFileManager);
    QFileSet *fs;
    switch (rType) {
        case Project:
            fs = &d->projects;
            break;
        case Folder:
            fs = &d->folders;
            break;
            break;
    }
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

QStringList QsFileManager::fetchRecent(QsFileManager::RecentType rType) const {
    Q_D(const QsFileManager);
    QStringList res;
    switch (rType) {
        case Project:
            res = d->projects.valid();
            break;
        case Folder:
            res = d->folders.valid();
            break;
        default:
            break;
    }
    return res;
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

QStringList QsFileManager::openFiles(const QString &title, const QString &filter, const QString &flag,
                                   QWidget *parent) {
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

QString QsFileManager::saveFile(const QString &title, const QString &filename, const QString &filter,
                              const QString &flag, QWidget *parent) {
    Q_D(QsFileManager);
    QFileInfo info(filename);
    if (info.isRelative() || !Sys::isDirExist(info.absolutePath())) {
        info.setFile(d->getLastOpenPath(flag) + Slash + info.fileName());
    }
    QString path =
        QFileDialog::getSaveFileName(parent, title, info.absoluteFilePath(), filter, nullptr);
    if (!path.isEmpty()) {
        d->saveLastOpenDir(flag, path);
    }
    return path;
}

QsFileManager::QsFileManager(QsFileManagerPrivate &d, QObject *parent) : QsAbstractManager(d, parent) {
    construct();
    d.init();
}
