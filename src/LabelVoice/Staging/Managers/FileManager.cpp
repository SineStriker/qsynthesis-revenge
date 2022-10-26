#include "FileManager.h"
#include "private/FileManager_p.h"

#include "Kernel/LvApplication.h"
#include "Serialization/QJsonFormatter.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

Q_SINGLETON_DECLARE(FileManager)

static const char Slash = '/';

static const char FILE_NAME_RECENT_JSON[] = "recent.json";
static const char KEY_NAME_RECENT_FILES[] = "files";
static const char KEY_NAME_RECENT_DIRS[] = "dirs";

FileManager::FileManager(QObject *parent) : FileManager(*new FileManagerPrivate(), parent) {
}

FileManager::~FileManager() {
}

void FileManager::load() {
    Q_D(FileManager);
    QFile file(qApp->appDataPath() + Slash + FILE_NAME_RECENT_JSON);
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
}

void FileManager::save() {
    Q_D(FileManager);
    QFile file(qApp->appDataPath() + Slash + FILE_NAME_RECENT_JSON);
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
}

void FileManager::commitRecent(FileManager::RecentType rType, FileManager::ChangeType cType,
                               const QString &filename) {
    Q_D(FileManager);
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

QStringList FileManager::fetchRecent(FileManager::RecentType rType) const {
    Q_D(const FileManager);
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

FileManager::FileManager(FileManagerPrivate &d, QObject *parent) : BasicManager(d, parent) {
    construct();
    d.init();
}
