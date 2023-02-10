#include "QsFileManager_p.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardPaths>

#include "Kernel/QsCoreConfig.h"
#include "QsLinq.h"
#include "QsSystem.h"

#include "QsCoreDecorator.h"

static const char SECTION_NAME_FILE_SYSTEM[] = "filesystem";
static const char KEY_NAME_RECENT_FILES[] = "files";
static const char KEY_NAME_RECENT_DIRS[] = "dirs";

static const char SECTION_NAME_FILE_DIALOG[] = "fileDialog";

static const char SECTION_NAME_DECORATOR[] = "decorator";
static const char KEY_NAME_LAST_LOCALE[] = "locale";
static const char KEY_NAME_LAST_THEME[] = "theme";

QsFileManagerPrivate::QsFileManagerPrivate() {
}

QsFileManagerPrivate::~QsFileManagerPrivate() {
}

void QsFileManagerPrivate::init() {
    fileRegMax = QsFileManager::Project;
    dirRegMax = QsFileManager::Folder;
    recentMap = {
        {
            fileRegMax,
            RecentDesc{KEY_NAME_RECENT_FILES, QFileSet(QFileSet::File)},
        },
        {
            dirRegMax,
            RecentDesc{KEY_NAME_RECENT_DIRS, QFileSet(QFileSet::Directory)},
        },
    };
}

bool QsFileManagerPrivate::load_helper(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray data(file.readAll());
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        return false;
    }

    QJsonObject objDoc = doc.object();

    QJsonObject::ConstIterator it;

    // Get file lists
    it = objDoc.find(SECTION_NAME_FILE_SYSTEM);
    if (it != objDoc.end() && it.value().isObject()) {
        QJsonObject obj = it.value().toObject();
        for (auto it2 = recentMap.begin(); it2 != recentMap.end(); ++it2) {
            RecentDesc &info = it2.value();
            auto it3 = obj.find(info.key);
            if (it3 == obj.end() || !it3.value().isArray()) {
                continue;
            }

            QJsonArray arr = it3.value().toArray();

            QStringList strs;
            for (auto it4 = arr.begin(); it4 != arr.end(); ++it4) {
                if (!it4->isString()) {
                    continue;
                }
                strs.append(it4->toString());
            }
            info.set.set(strs);
        }
    }

    // Get file dialog history
    it = objDoc.find(SECTION_NAME_FILE_DIALOG);
    if (it != objDoc.end() && it.value().isObject()) {
        QJsonObject obj = it.value().toObject();
        for (auto it2 = obj.begin(); it2 != obj.end(); ++it2) {
            if (it2->isString()) {
                lastOpenPaths.insert(it2.key(), it2.value().toString());
            }
        }
    }

    // Get saved locale and theme
    it = objDoc.find(SECTION_NAME_DECORATOR);
    if (it != objDoc.end() && it.value().isObject()) {
        QJsonObject obj = it.value().toObject();
        auto it2 = obj.find(KEY_NAME_LAST_LOCALE);
        if (it2 != obj.end() && it2->isString()) {
            qIDec->setLocale(it2->toString());
        }
        it2 = obj.find(KEY_NAME_LAST_THEME);
        if (it2 != obj.end() && it2->isString()) {
            qIDec->setTheme(it2->toString());
        }
    }

    return true;
}

bool QsFileManagerPrivate::save_helper(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
    }

    QJsonObject recentObj;
    for (auto it = recentMap.begin(); it != recentMap.end(); ++it) {
        const auto &info = it.value();
        recentObj.insert(info.key, QJsonArray::fromStringList(info.set.valid()));
    }

    QJsonObject fdObj;
    for (auto it = lastOpenPaths.begin(); it != lastOpenPaths.end(); ++it) {
        fdObj.insert(it.key(), it.value());
    }

    QJsonDocument doc;
    QJsonObject obj{
        {
            SECTION_NAME_FILE_SYSTEM,
            recentObj,
        },
        {
            SECTION_NAME_FILE_DIALOG,
            fdObj,
        },
        {
            SECTION_NAME_DECORATOR,
            QJsonObject{
                {KEY_NAME_LAST_LOCALE, qIDec->locale()},
                {KEY_NAME_LAST_THEME, qIDec->theme()},
            },
        },
    };
    doc.setObject(obj);
    file.write(doc.toJson());
    file.close();

    return true;
}

QString QsFileManagerPrivate::getLastOpenPath(const QString &type) {
    auto it = lastOpenPaths.find(type);
    if (it == lastOpenPaths.end()) {
        it = lastOpenPaths.insert(
            type, QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    } else if (!QsFs::isDirExist(it.value())) {
        it.value() = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }
    return it.value();
}

void QsFileManagerPrivate::saveLastOpenDir(const QString &type, const QString &path, bool upper) {
    lastOpenPaths.insert(type, upper ? QsFs::PathFindDirPath(path) : path);
}
