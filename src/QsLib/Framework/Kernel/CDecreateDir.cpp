#include "CDecreateDir.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

#include "CDecorator.h"

static const char KEY_NAME_LOCALES[] = "locales";
static const char KEY_NAME_THEMES[] = "themes";

static const char KEY_NAME_SUBSECTION_KEY[] = "key";
static const char KEY_NAME_SUBSECTION_DIR[] = "dir";
static const char KEY_NAME_SUBSECTION_FILES[] = "files";

static const char Slash = '/';

CDecreateDir::CDecreateDir() {
    vars.addHash(QSimpleVarExp::SystemValues());
}

CDecreateDir::CDecreateDir(const QString &dir) : CDecreateDir() {
    setDir(dir);
}

CDecreateDir::~CDecreateDir() {
    unload();
}

void CDecreateDir::setDir(const QString &dir) {
    vars.add("FILEPATH", dir);
}

bool CDecreateDir::loadDefault(const QString &binName) {
    return load(dir + Slash + binName + ".res.json");
}

bool CDecreateDir::load(const QString &filename) {
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

    auto it = objDoc.find(KEY_NAME_LOCALES);
    while (it != objDoc.end() && it->isObject()) {
        auto obj = it->toObject();

        // Find key
        QString key;
        auto it2 = obj.find(KEY_NAME_SUBSECTION_KEY);
        if (it2 == obj.end() || !it2->isString()) {
            break;
        }
        key = it2->toString();
        if (key.isEmpty()) {
            continue;
        }

        // Find dir and parse
        QString subdir;
        it2 = obj.find(KEY_NAME_SUBSECTION_DIR);
        if (it2 != obj.end() || !it2->isString()) {
            subdir = dir;
        } else {
            subdir = vars.parse(it2->toString());
            if (subdir.isEmpty()) {
                subdir = dir;
            }
        }

        // Add files
        it2 = obj.find(KEY_NAME_SUBSECTION_FILES);
        if (it2 == obj.end() || !it2->isObject()) {
            break;
        }
        auto objFiles = it2->toObject();

        QHash<QLocale, QStringList> paths;
        for (auto it3 = objFiles.begin(); it3 != objFiles.end(); ++it3) {
            const auto &itemKey = it3.key();
            if (!it3->isArray()) {
                continue;
            }
            auto arr = it3->toArray();

            QStringList files;
            for (const auto &item : qAsConst(arr)) {
                if (item.isString()) {
                    files.append(subdir + Slash + item.toString());
                }
            }
            if (files.isEmpty()) {
                continue;
            }

            paths.insert(QLocale(itemKey), files);
        }

        if (paths.isEmpty()) {
            break;
        }

        localeKey = key;
        qIDec->addLocale(key, paths);
        break;
    }

    it = objDoc.find(KEY_NAME_THEMES);
    while (it != objDoc.end() && it->isObject()) {
        auto obj = it->toObject();

        // Find key
        QString key;
        auto it2 = obj.find(KEY_NAME_SUBSECTION_KEY);
        if (it2 == obj.end() || !it2->isString()) {
            break;
        }
        key = it2->toString();
        if (key.isEmpty()) {
            continue;
        }

        // Find dir and parse
        QString subdir;
        it2 = obj.find(KEY_NAME_SUBSECTION_DIR);
        if (it2 != obj.end() || !it2->isString()) {
            subdir = dir;
        } else {
            subdir = vars.parse(it2->toString());
            if (subdir.isEmpty()) {
                subdir = dir;
            }
        }

        // Add files
        it2 = obj.find(KEY_NAME_SUBSECTION_FILES);
        if (it2 == obj.end() || !it2->isObject()) {
            break;
        }
        auto objFiles = it2->toObject();

        QMap<QString, QStringList> paths;
        for (auto it3 = objFiles.begin(); it3 != objFiles.end(); ++it3) {
            const auto &itemKey = it3.key();
            if (!it3->isArray()) {
                continue;
            }
            auto arr = it3->toArray();

            QStringList files;
            for (const auto &item : qAsConst(arr)) {
                if (item.isString()) {
                    files.append(subdir + Slash + item.toString());
                }
            }
            if (files.isEmpty()) {
                continue;
            }

            paths.insert(itemKey, files);
        }

        if (paths.isEmpty()) {
            break;
        }

        themeKey = key;
        qIDec->addThemeConfig(key, paths);
        break;
    }

    return true;
}

void CDecreateDir::unload() {
    if (!localeKey.isEmpty()) {
        qIDec->removeLocale(localeKey);
        localeKey.clear();
    }

    if (!themeKey.isEmpty()) {
        qIDec->removeThemeConfig(themeKey);
        themeKey.clear();
    }
}