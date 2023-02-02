#include "CDecorateDir.h"
#include "private/CDecorateDir_p.h"

#include "CDecorator.h"

static const char KEY_NAME_THEMES[] = "themes";

static const char Slash = '/';

CDecorateDir::CDecorateDir() : CDecorateDir(*new CDecorateDirPrivate()) {
}

CDecorateDir::CDecorateDir(const QString &dir) : CDecorateDir() {
    setDir(dir);
}

CDecorateDir::~CDecorateDir() {
}

bool CDecorateDir::load(const QString &filename) {
    if (!QsLocaleDir::load(filename)) {
        return false;
    }

    Q_D(CDecorateDir);

    // Parse themes
    auto it = d->rootItems.find(KEY_NAME_THEMES);
    while (it != d->rootItems.end()) {
        const auto &item = it.value();
        QString key = item.key;
        if (key.isEmpty()) {
            break;
        }

        // Find dir and parsedir;
        QString subdir = vars.parse(item.dir);
        if (subdir.isEmpty()) {
            subdir = d->dir;
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
        d->themeKey = key;
        break;
    }

    return true;
}

void CDecorateDir::unload() {
    Q_D(CDecorateDir);
    QsLocaleDir::unload();
    d->unloadTheme();
}

CDecorateDir::CDecorateDir(CDecorateDirPrivate &d) : QsLocaleDir(d) {
    d.init();
}
