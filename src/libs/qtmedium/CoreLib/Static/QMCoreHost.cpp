#include "QMCoreHost.h"
#include "private/QMCoreHost_p.h"
#include "private/QMNamespace_p.h"

#include "QMSystem.h"

#include <QCoreApplication>
#include <QDebug>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSettings>

Q_SINGLETON_DECLARE(QMCoreHost);

static bool readJsonFile(QIODevice &device, QSettings::SettingsMap &map) {
    bool ret = false;

    QJsonParseError error;
    map = QJsonDocument::fromJson(device.readAll(), &error).toVariant().toMap();

    if (error.error == QJsonParseError::NoError)
        ret = true;

    return ret;
}

static bool writeJsonFile(QIODevice &device, const QSettings::SettingsMap &map) {
    bool ret = false;

    QJsonDocument jsonDocument = QJsonDocument::fromVariant(QVariant::fromValue(map));
    if (device.write(jsonDocument.toJson()) != -1)
        ret = true;

    return ret;
}

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

    QHash<QString, QStringList> confValues;

    // Read qtmedium.json
    {
        QString appDir =
#ifdef Q_OS_MAC
            qApp->applicationDirPath() + "/../Resources"
#else
            qApp->applicationDirPath()
#endif
            ;

        QFile file(appDir + "/qtmedium.conf.json");
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
    QString appDir =
#ifdef Q_OS_MAC
        qApp->applicationDirPath() + "/.."
#else
        qApp->applicationDirPath()
#endif
        ;

    auto prefix = confValues.value("Prefix", {appDir}).front();
    if (QMFs::isPathRelative(prefix)) {
        prefix = appDir + "/" + prefix;
    }
    prefix = QFileInfo(prefix).canonicalFilePath();

    libDir = confValues
                 .value("Libraries",
                        {
#ifdef Q_OS_MAC
                            "Frameworks"
#else
                            "lib"
#endif
                        })
                 .front();
    if (QMFs::isPathRelative(libDir)) {
        libDir = prefix + "/" + libDir;
    }

    shareDir = confValues
                   .value("Share",
                          {
#ifdef Q_OS_MAC
                              "Resources"
#else
                              "share"
#endif
                          })
                   .front();
    if (QMFs::isPathRelative(shareDir)) {
        shareDir = prefix + "/" + shareDir;
    }

    QStringList plugins = confValues.value("Plugins", {});
    for (auto path : qAsConst(plugins)) {
        if (QMFs::isPathRelative(path)) {
            path = prefix + "/" + path;
        }
        QCoreApplication::addLibraryPath(path);
    }

    QStringList translations = confValues.value("Translations", {});
    for (auto path : qAsConst(translations)) {
        if (QMFs::isPathRelative(path)) {
            path = prefix + "/" + path;
        }
        translationPaths.append(path);
    }

    QStringList themes = confValues.value("Themes", {});
    for (auto path : qAsConst(themes)) {
        if (QMFs::isPathRelative(path)) {
            path = prefix + "/" + path;
        }
        themePaths.append(path);
    }

    // Init factory
    fac.reset(createFactory());

    // Create instances
    s_dec = fac->createDecorator(q);
    qmDebug() << "qtmedium: " << s_dec->metaObject()->className() << " initializing.";

    s_cs = fac->createConsole(q);
    qmDebug() << "qtmedium: " << s_cs->metaObject()->className() << " initializing.";

    q->connect(qApp, &QCoreApplication::aboutToQuit, q, &QMCoreHost::_q_appAboutToQuit);

    // Add translation paths
    for (const auto &path : qAsConst(translationPaths))
        s_dec->addTranslationPath(path);
}

QMCoreFactory *QMCoreHostPrivate::createFactory() {
    return new QMCoreFactory();
}

QMCoreHost::QMCoreHost(QObject *parent) : QMCoreHost(*new QMCoreHostPrivate(), parent) {
}

QMCoreHost::~QMCoreHost() {
}

bool QMCoreHost::isAboutToQuit() const {
    Q_D(const QMCoreHost);
    return d->isAboutToQuit;
}

QSettings::Format QMCoreHost::settingsJsonFormat() const {
    Q_D(const QMCoreHost);
    return d->settingsJsonFormat;
}

QString QMCoreHost::appDataDir() const {
    Q_D(const QMCoreHost);
    return d->appDataDir;
}

void QMCoreHost::setAppDataDir(const QString &dir) {
    Q_D(QMCoreHost);
    d->appDataDir = dir;
}

QString QMCoreHost::tempDir() const {
    Q_D(const QMCoreHost);
    return d->tempDir;
}

void QMCoreHost::setTempDir(const QString &dir) {
    Q_D(QMCoreHost);
    d->tempDir = dir;
}

QString QMCoreHost::libDir() const {
    Q_D(const QMCoreHost);
    return d->libDir;
}

void QMCoreHost::setLibDir(const QString &dir) {
    Q_D(QMCoreHost);
    d->libDir = dir;
}

QString QMCoreHost::shareDir() const {
    Q_D(const QMCoreHost);
    return d->shareDir;
}

void QMCoreHost::setShareDir(const QString &dir) {
    Q_D(QMCoreHost);
    d->shareDir = dir;
}

bool QMCoreHost::createDataAndTempDirs() const {
    auto func = [](const QString &path) {
        qDebug() << "qmcorehost:" << (QMFs::isDirExist(path) ? "find" : "create") << "directory" << path;
        if (!QMFs::mkDir(path)) {
            qmCon->MsgBox(nullptr, QMCoreConsole::Critical, qAppName(),
                          QString("Failed to create %1 directory!").arg(QMFs::PathFindFileName(path)));
            return false;
        }
        return true;
    };

    if (!func(appDataDir())) {
        return false;
    }

    if (!func(tempDir())) {
        return false;
    }

    return true;
}

QMCoreHost::QMCoreHost(QMCoreHostPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}

void QMCoreHost::_q_appAboutToQuit() {
    Q_D(QMCoreHost);
    d->isAboutToQuit = true;
}