#include "QMLocaleDir.h"
#include "private/QMLocaleDir_p.h"

#include "QMSystem.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

static const char KEY_NAME_CONFIG[] = "config";

static const char KEY_NAME_CONFIG_VERSION[] = "config";
static const char KEY_NAME_CONFIG_PARENT[] = "parent";
static const char KEY_NAME_CONFIG_VALUES[] = "vars";

bool QMLocaleDir::AutoDetectLocales = true;

QMLocaleDir::QMLocaleDir() : QMLocaleDir(*new QMLocaleDirPrivate()) {
}

QMLocaleDir::~QMLocaleDir() {
}

QString QMLocaleDir::dir() const {
    Q_D(const QMLocaleDir);
    return d->dir;
}

QMLocaleDir::QMLocaleDir(const QString &dir) : QMLocaleDir() {
    setDir(dir);
}

void QMLocaleDir::setDir(const QString &dir) {
    Q_D(QMLocaleDir);
    d->dir = dir;
    vars.add("FILEPATH", dir);
}

bool QMLocaleDir::loadDefault(const QString &name) {
    Q_D(QMLocaleDir);
    return load(QString("%1/%2.res.json").arg(d->dir, name));
}

bool QMLocaleDir::load(const QString &filename) {
    Q_D(QMLocaleDir);

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug().noquote()
            << QString("load_locale: failed to open decoration config %1").arg(QMFs::PathFindFileName(filename));
        return false;
    }

    QByteArray data(file.readAll());
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        qDebug().noquote() << QString("load_locale: invalid decoration config %1").arg(QMFs::PathFindFileName(filename));
        return false;
    }

    qDebug().noquote() << QString("load_locale: register decoration config %1").arg(QMFs::PathFindFileName(filename));

    auto objDoc = doc.object();
    auto it = objDoc.find(KEY_NAME_CONFIG);
    if (it != objDoc.end() && it->isObject()) {
        auto objConfig = it->toObject();
        auto it2 = objConfig.find(KEY_NAME_CONFIG_VERSION);
        if (it2 != objConfig.end() && it2->isString()) {
            d->version = it2->toString();
            vars.add("VERSION", d->parent);
        }
        it2 = objConfig.find(KEY_NAME_CONFIG_PARENT);
        if (it2 != objConfig.end() && it2->isString()) {
            d->parent = it2->toString();
            vars.add("PARENT", d->parent);
        }
        it2 = objConfig.find(KEY_NAME_CONFIG_VALUES);
        if (it2 != objConfig.end() && it2->isObject()) {
            auto obj = it2->toObject();
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                if (it->isString()) {
                    vars.add(it.key(), it->toString());
                }
            }
        }
    }

    return d->loadNext(objDoc);
}

void QMLocaleDir::unload() {
    Q_D(QMLocaleDir);
    d->unloadNext();
}

QMLocaleDir::QMLocaleDir(QMLocaleDirPrivate &d) : d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}