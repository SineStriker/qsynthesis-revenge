#include "QsLocaleDir_p.h"

#include "../QsCoreDecorator.h"

#define _CURRENT_MAP QHash
#define _CURRENT_TRASACTION QHashTransaction

#include "QMapTransaction.h"

#undef _CURRENT_MAP
#undef _CURRENT_TRASACTION

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

static const char KEY_NAME_SUBSECTION_KEY[] = "key";
static const char KEY_NAME_SUBSECTION_DIR[] = "dir";
static const char KEY_NAME_SUBSECTION_FILES[] = "files";

static const char KEY_NAME_LOCALES[] = "locales";

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

bool QsLocaleDirPrivate::loadNext(const QJsonObject &objDoc) {
    Q_Q(QsLocaleDir);

    auto parse = [&](const QJsonObject &obj) {
        QsLocaleDirPrivate::RootItem cur;
        if (!loadRootItems(obj, &cur)) {
            return;
        }

        cur.key = q->vars.parse(cur.key);
        if (cur.key.isEmpty()) {
            return;
        }

        // Start transaction
        {
            QMapTransaction<QString, QString> tx1(&q->vars.Variables);
            tx1.insert("CURRENT_KEY", cur.key);

            // Find dir and parse dir;
            cur.dir = q->vars.parse(cur.dir);
            if (cur.dir.isEmpty()) {
                cur.dir = dir;
            }

            tx1.insert("CURRENT_DIR", cur.dir);

            // Add files
            QMap<QString, QStringList> paths;
            for (auto it3 = cur.files.begin(); it3 != cur.files.end(); ++it3) {
                QStringList files = it3.value();
                if (files.isEmpty()) {
                    continue;
                }
                for (auto &file : files) {
                    file = q->vars.parse(cur.dir + Slash + file);
                }
                paths.insert(it3.key(), files);
            }
            if (!paths.isEmpty()) {
                qIDec->addLocale(cur.key, paths);
                locales.append(cur);
            }
        }
    };

    // Parse locales
    auto it = objDoc.find(KEY_NAME_LOCALES);
    if (it != objDoc.end()) {
        if (it->isArray()) {
            QJsonArray arr = it->toArray();
            for (const auto &item : qAsConst(arr)) {
                if (item.isObject()) {
                    parse(item.toObject());
                }
            }
        } else if (it->isObject()) {
            parse(it->toObject());
        }
    }

    return true;
}

void QsLocaleDirPrivate::unloadNext() {
    unloadLocale();
}

void QsLocaleDirPrivate::unloadLocale() {
    // Remove locales
    if (!locales.isEmpty()) {
        for (const auto &item : qAsConst(locales)) {
            qIDec->removeLocale(item.key);
        }
        locales.clear();
    }
}

bool QsLocaleDirPrivate::loadRootItems(const QJsonObject &obj, RootItem *out) {
    RootItem cur;

    // Find key
    auto it2 = obj.find(KEY_NAME_SUBSECTION_KEY);
    if (it2 == obj.end() || !it2->isString()) {
        return false;
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
        return false;
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

    *out = std::move(cur);
    return true;
}