#include "QsLocaleDir_p.h"

#include "../QsCoreDecorator.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

static const char KEY_NAME_SUBSECTION_KEY[] = "key";
static const char KEY_NAME_SUBSECTION_DIR[] = "dir";
static const char KEY_NAME_SUBSECTION_FILES[] = "files";

static const char Slash = '/';

QsLocaleDirPrivate::QsLocaleDirPrivate() {
}

QsLocaleDirPrivate::~QsLocaleDirPrivate() {
    Q_Q(QsLocaleDir);
    if (q->autoRemove)
        unloadLocale();
}

void QsLocaleDirPrivate::init() {
    Q_Q(QsLocaleDir);
    q->autoRemove = false;
    q->vars.addHash(QSimpleVarExp::SystemValues());
}

bool QsLocaleDirPrivate::loadRootItems(const QString &filename) {
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

    for (auto it = objDoc.begin(); it != objDoc.end(); ++it) {
        if (!it->isObject()) {
            continue;
        }

        RootItem cur;

        auto obj = it->toObject();

        // Find key
        auto it2 = obj.find(KEY_NAME_SUBSECTION_KEY);
        if (it2 == obj.end() || !it2->isString()) {
            continue;
        }
        cur.key = it2->toString();

        // Find dir
        it2 = obj.find(KEY_NAME_SUBSECTION_DIR);
        if (it2 != obj.end() || !it2->isString()) {
            cur.dir = it2->toString();
        }

        // Add files
        it2 = obj.find(KEY_NAME_SUBSECTION_FILES);
        if (it2 == obj.end() || !it2->isObject()) {
            continue;
        }
        auto objFiles = it2->toObject();

        for (auto it3 = objFiles.begin(); it3 != objFiles.end(); ++it3) {
            const auto &itemKey = it3.key();
            if (!it3->isArray()) {
                continue;
            }
            auto arr = it3->toArray();

            QStringList files;
            for (const auto &item : qAsConst(arr)) {
                if (item.isString()) {
                    files.append(item.toString());
                }
            }
            if (files.isEmpty()) {
                continue;
            }

            cur.files.insert(itemKey, files);
        }

        rootItems.insert(it.key(), cur);
    }
    return true;
}

void QsLocaleDirPrivate::unloadLocale() {
    // Remove locale
    if (!localeKey.isEmpty()) {
        qIDec->removeLocale(localeKey);
        localeKey.clear();
    }
}