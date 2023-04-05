#include "QMCoreHost_p.h"

#include "QMSystem.h"
#include "QSettingsJsonFormat.h"
#include "private/QMNamespace_p.h"

#include <QCoreApplication>
#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QMCoreHostPrivate::QMCoreHostPrivate() {
    isAboutToQuit = false;

    // This is necessary for MacOS platforms, so that QIcon will return a
    // pixmap with correct devicePixelRatio when using QIcon::pixmap().
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    Register_QMNamespace();

    settingsJsonFormat = QSettings::registerFormat("json", readJsonFile, writeJsonFile);
}

QMCoreHostPrivate::~QMCoreHostPrivate() {
}

void QMCoreHostPrivate::init() {
    Q_Q(QMCoreHost);

    // Init basic directories
    appDataDir = QMFs::appDataPath() + "/ChorusKit/" + qAppName();
    tempDir = QDir::tempPath() + "/ChorusKit/" + qAppName();

    // Read qtmedium.json
    {
        QFile file(qApp->applicationDirPath() + "/qtmedium.conf.json");
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "qtmedium: configuration file not found";
            goto finish_conf;
        }
        QByteArray data(file.readAll());
        file.close();

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            qDebug() << "qtmedium: configuration file not valid";
            goto finish_conf;
        }

        qDebug() << "qtmedium: load configuration file success";

        auto obj = doc.object();
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            QStringList res;
            if (it->isArray()) {
                auto arr = it->toArray();
                for (const auto &item : qAsConst(arr)) {
                    if (item.isString()) {
                        res.append(item.toString());
                    }
                }
            } else if (it->isString()) {
                res.append(it->toString());
            } else if (it->isDouble()) {
                res.append(QString::number(it->toDouble()));
            }
            if (res.isEmpty()) {
                continue;
            }
            confValues.insert(it.key(), res);
        }
    }

finish_conf:
    QString appDir = qApp->applicationDirPath();

    confPrefix = confValues.value("Prefix", {appDir}).front();
    if (QMFs::isPathRelative(confPrefix)) {
        confPrefix = appDir + "/" + confPrefix;
    }
    confPrefix = QFileInfo(confPrefix).canonicalFilePath();

    libDir = confValues.value("Libraries", {"lib"}).front();
    if (QMFs::isPathRelative(libDir)) {
        libDir = confPrefix + "/" + libDir;
    }

    shareDir = confValues.value("Share", {"share"}).front();
    if (QMFs::isPathRelative(shareDir)) {
        shareDir = confPrefix + "/" + shareDir;
    }

    QStringList plugins = confValues.value("Plugins", {});
    for (auto path : qAsConst(plugins)) {
        if (QMFs::isPathRelative(path)) {
            path = confPrefix + "/" + path;
        }
        QCoreApplication::addLibraryPath(path);
    }

    // Init factory
    fac.reset(createFactory());

    // Create instances
    s_dec = fac->createDecorator(q);
    qmDebug() << "qtmedium: " << s_dec->metaObject()->className() << " initializing.";

    s_cs = fac->createConsole(q);
    qmDebug() << "qtmedium: " << s_cs->metaObject()->className() << " initializing.";

    q->connect(qApp, &QCoreApplication::aboutToQuit, q, &QMCoreHost::_q_appAboutToQuit);
}

QMCoreFactory *QMCoreHostPrivate::createFactory() {
    return new QMCoreFactory();
}