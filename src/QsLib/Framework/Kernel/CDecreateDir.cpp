#include "CDecreateDir.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

#include "CDecorator.h"

static const char KEY_NAME_THEMES[] = "themes";

static const char Slash = '/';

CDecreateDir::CDecreateDir() : QsLocaleDir() {
}

CDecreateDir::CDecreateDir(const QString &dir) : QsLocaleDir(dir) {
}

CDecreateDir::~CDecreateDir() {
}

bool CDecreateDir::load(const QString &filename) {
    if (!QsLocaleDir::load(filename)) {
        return false;
    }

    // Parse themes
    auto it = rootItems.find(KEY_NAME_THEMES);
    while (it != rootItems.end()) {
        const auto &item = it.value();
        QString key = item.key;
        if (key.isEmpty()) {
            break;
        }

        // Find dir and parsedir;
        QString subdir = vars.parse(item.dir);
        if (subdir.isEmpty()) {
            subdir = dir;
        }

        // Add files
        QMap<QString, QStringList> paths;
        for (auto it3 = item.files.begin(); it3 != item.files.end(); ++it3) {
            QStringList files = it3.value();
            if (files.isEmpty()) {
                continue;
            }
            for (auto &file : files) {
                file = subdir + Slash + file;
            }
            paths.insert(it3.key(), files);
        }

        if (paths.isEmpty()) {
            break;
        }

        qIDec->addThemeConfig(key, paths);
        themeKey = key;
        unloaders.append(std::bind(&CDecreateDir::unloadTheme, this));
        break;
    }

    return true;
}

void CDecreateDir::unloadTheme() {
    if (!themeKey.isEmpty()) {
        qIDec->removeThemeConfig(themeKey);
        themeKey.clear();
    }
}
