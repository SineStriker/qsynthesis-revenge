#include "QsLocaleDir.h"
#include "private/QsLocaleDir_p.h"

#include "QsCoreDecorator.h"
#include "QsSystem.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

static const char KEY_NAME_CONFIG[] = "config";

static const char KEY_NAME_CONFIG_VERSION[] = "config";
static const char KEY_NAME_CONFIG_PARENT[] = "parent";
static const char KEY_NAME_CONFIG_VALUES[] = "vars";

bool QsLocaleDir::AutoDetectLocales = true;

QsLocaleDir::QsLocaleDir() : QsLocaleDir(*new QsLocaleDirPrivate()) {
}

QsLocaleDir::~QsLocaleDir() {
}

QString QsLocaleDir::dir() const {
    Q_D(const QsLocaleDir);
    return d->dir;
}

QsLocaleDir::QsLocaleDir(const QString &dir) : QsLocaleDir() {
    setDir(dir);
}

void QsLocaleDir::setDir(const QString &dir) {
    Q_D(QsLocaleDir);
    d->dir = dir;
    vars.add("FILEPATH", dir);
}

bool QsLocaleDir::loadDefault(const QString &name) {
    Q_D(QsLocaleDir);
    return load(QString("%1/%2.res.json").arg(d->dir, name));
}

bool QsLocaleDir::load(const QString &filename) {
    Q_D(QsLocaleDir);

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug().noquote() << QString("load_locale: failed to open dir-desc file %1")
                                  .arg(QsFs::PathFindFileName(filename));
        return false;
    }

    QByteArray data(file.readAll());
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        qDebug().noquote() << QString("load_locale: invalid dir-desc file %1")
                                  .arg(QsFs::PathFindFileName(filename));
        return false;
    }

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

void QsLocaleDir::unload() {
    Q_D(QsLocaleDir);
    d->unloadNext();
}

QsLocaleDir::QsLocaleDir(QsLocaleDirPrivate &d) : d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
