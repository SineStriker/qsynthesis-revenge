#include "QsLocaleDir.h"
#include "private/QsLocaleDir_p.h"

#include "QsCoreDecorator.h"

static const char KEY_NAME_LOCALES[] = "locales";

static const char Slash = '/';

QsLocaleDir::QsLocaleDir() : QsLocaleDir(*new QsLocaleDirPrivate()) {
}

QsLocaleDir::~QsLocaleDir() {
}

QsLocaleDir::QsLocaleDir(const QString &dir) : QsLocaleDir() {
    setDir(dir);
}

void QsLocaleDir::setDir(const QString &dir) {
    Q_D(QsLocaleDir);
    d->dir = dir;
    vars.add("FILEPATH", dir);
}

bool QsLocaleDir::loadDefault(const QString &binName) {
    Q_D(QsLocaleDir);
    return load(d->dir + Slash + binName + ".res.json");
}

bool QsLocaleDir::load(const QString &filename) {
    Q_D(QsLocaleDir);

    if (!d->loadRootItems(filename)) {
        return false;
    }

    // Parse locales
    auto it = d->rootItems.find(KEY_NAME_LOCALES);
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

        qIDec->addLocale(key, paths);
        d->localeKey = key;
        break;
    }

    return true;
}

void QsLocaleDir::unload() {
    Q_D(QsLocaleDir);
    d->unloadLocale();
}

QsLocaleDir::QsLocaleDir(QsLocaleDirPrivate &d) : d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}