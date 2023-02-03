#include "CDecorateDir_p.h"

#include "../CDecorator.h"

#define _CURRENT_MAP QHash
#define _CURRENT_TRASACTION QHashTransaction

#include "QMapTransaction.h"

#undef _CURRENT_MAP
#undef _CURRENT_TRASACTION

#include <QJsonArray>

static const char KEY_NAME_THEMES[] = "themes";

static const char Slash = '/';

CDecorateDirPrivate::CDecorateDirPrivate() {
}

CDecorateDirPrivate::~CDecorateDirPrivate() {
    Q_Q(QsLocaleDir);
    if (q->autoRemove)
        unloadTheme();
}

void CDecorateDirPrivate::init() {
}

bool CDecorateDirPrivate::loadNext(const QJsonObject &objDoc) {
    if (!QsLocaleDirPrivate::loadNext(objDoc)) {
        return false;
    }

    Q_Q(CDecorateDir);

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
                qIDec->addThemeConfig(cur.key, paths);
                themes.append(cur);
            }
        }
    };

    // Parse themes
    auto it = objDoc.find(KEY_NAME_THEMES);
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

void CDecorateDirPrivate::unloadNext() {
    QsLocaleDirPrivate::unloadNext();
    unloadTheme();
}

void CDecorateDirPrivate::unloadTheme() {
    // Remove themes
    if (!themes.isEmpty()) {
        for (const auto &item : qAsConst(themes)) {
            qIDec->removeThemeConfig(item.key);
        }
        themes.clear();
    }
}