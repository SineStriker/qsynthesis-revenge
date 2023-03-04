#include "QMLocaleDir_p.h"

#include "../QMCoreDecorator.h"
#include "QMMapTransaction.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

static const char KEY_NAME_SUBSECTION_KEY[] = "key";
static const char KEY_NAME_SUBSECTION_DIR[] = "dir";
static const char KEY_NAME_SUBSECTION_FILES[] = "files";

static const char KEY_NAME_LOCALES[] = "locales";

static const char Slash = '/';

static QMap<QString, QStringList> findLocales(const QString &prefix, const QString &dirname) {
    QDir dir(dirname);
    if (!dir.exists()) {
        return {};
    }

    QRegularExpression reg("(\\w+?)_(\\w{2})(_\\w+|)");
    auto files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    QMap<QString, QStringList> res;

    for (auto it = files.begin(); it != files.end(); ++it) {
        if (it->suffix().compare("qm", Qt::CaseInsensitive) != 0) {
            continue;
        }

        auto match = reg.match(it->fileName());
        if (!match.hasMatch() || (!prefix.isEmpty() && match.captured() != prefix)) {
            continue;
        }
        res[match.captured(2) + match.captured(3)].append(it->absoluteFilePath());
    }

    return res;
}

QMLocaleDirPrivate::QMLocaleDirPrivate() {
}

QMLocaleDirPrivate::~QMLocaleDirPrivate() {
    Q_Q(QMLocaleDir);
    if (q->autoRemove)
        unloadLocale();
}

void QMLocaleDirPrivate::init() {
    Q_Q(QMLocaleDir);
    q->autoRemove = false;
    q->vars.addHash(QMSimpleVarExp::SystemValues());
}

bool QMLocaleDirPrivate::loadNext(const QJsonObject &objDoc) {
    Q_Q(QMLocaleDir);

    auto parse = [&](const QJsonObject &obj) {
        QMLocaleDirPrivate::RootItem cur;
        if (!loadRootItems(obj, &cur)) {
            return;
        }

        cur.key = q->vars.parse(cur.key);
        if (cur.key.isEmpty()) {
            return;
        }

        // Start transaction
        {
            QMapTransaction<QHash, QString, QString> tx1(&q->vars.Variables);
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

            // Auto detect locales if empty
            if (paths.isEmpty() && q->AutoDetectLocales) {
                paths = findLocales(QString(), cur.dir);
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

void QMLocaleDirPrivate::unloadNext() {
    unloadLocale();
}

void QMLocaleDirPrivate::unloadLocale() {
    // Remove locales
    if (!locales.isEmpty()) {
        for (const auto &item : qAsConst(locales)) {
            qIDec->removeLocale(item.key);
        }
        locales.clear();
    }
}

bool QMLocaleDirPrivate::loadRootItems(const QJsonObject &obj, RootItem *out) {
    RootItem cur;

    // Find key
    auto it2 = obj.find(KEY_NAME_SUBSECTION_KEY);
    if (it2 == obj.end() || !it2->isString()) {
        return false;
    }
    cur.key = it2->toString();

    // Find dir
    it2 = obj.find(KEY_NAME_SUBSECTION_DIR);
    if (it2 != obj.end() && it2->isString()) {
        cur.dir = it2->toString();
    }

    // Add files
    it2 = obj.find(KEY_NAME_SUBSECTION_FILES);
    if (it2 != obj.end() && !it2->isObject()) {
        return false;
    }

    if (it2 != obj.end()) {
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
    }

    *out = std::move(cur);
    return true;
}
