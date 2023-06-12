#include "QMCoreAppExtension.h"
#include "private/QMCoreAppExtension_p.h"

#include "QMSystem.h"

#include <QCoreApplication>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMessageLogger>
#include <QSettings>

Q_LOGGING_CATEGORY(qAppExtLog, "qtmediate")

Q_SINGLETON_DECLARE(QMCoreAppExtension);

#ifdef Q_OS_MAC
#    define QT_CONFIG_FILE_DIR  appUpperDir() + "/Resources"
#    define QT_CONFIG_BASE_DIR  appUpperDir()
#    define DEFAULT_LIBRARY_DIR "Frameworks"
#    define DEFAULT_SHARE_DIR   "Resources"
#else
#    define QT_CONFIG_FILE_DIR      QCoreApplication::applicationDirPath()
#    define QT_CONFIG_BASE_DIR      QCoreApplication::applicationDirPath()
#    define DEFAULT_LIBRARY_DIR     "lib"
#    define DEFAULT_SHARE_DIR       "share"
#endif

static QString appUpperDir() {
    static QString dir = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/..");
    return dir;
}

QMCoreConsole *QMCoreInitFactory::createConsole(QObject *parent) {
    return new QMCoreConsole(parent);
}

QMCoreDecoratorV2 *QMCoreInitFactory::createDecorator(QObject *parent) {
    return new QMCoreDecoratorV2(parent);
}

QMCoreAppExtensionPrivate::QMCoreAppExtensionPrivate() {
    isAboutToQuit = false;
}

QMCoreAppExtensionPrivate::~QMCoreAppExtensionPrivate() {
}

void QMCoreAppExtensionPrivate::init() {
    Q_Q(QMCoreAppExtension);

    // Basic directories
    appDataDir = QMFs::appDataPath() + "/ChorusKit/" + qApp->applicationName();
    tempDir = QDir::tempPath() + "/ChorusKit/" + qApp->applicationName();

    QHash<QString, QStringList> confValues;

    // Read qtmediate.json
    {

        QFile file(QT_CONFIG_FILE_DIR + "/qtmediate.json");
        if (!file.open(QIODevice::ReadOnly)) {
            qCDebug(qAppExtLog) << "configuration file not found";
            goto finish_conf;
        }

        QByteArray data(file.readAll());
        file.close();

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            qCDebug(qAppExtLog) << "configuration file not valid";
            goto finish_conf;
        }

        qCDebug(qAppExtLog) << "load configuration file success";

        auto obj = doc.object();
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            QStringList res;

            if (it.key() == "AppFont") {
                if (it->isString()) {
                    appFont.insert("Family", it->toString());
                } else if (it->isObject()) {
                    appFont = it->toObject();
                }
                continue;
            }

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

    auto prefix = confValues.value("Prefix", {appUpperDir()}).front();
    if (QMFs::isPathRelative(prefix)) {
        prefix = QT_CONFIG_BASE_DIR + "/" + prefix;
    }
    prefix = QFileInfo(prefix).canonicalFilePath();

    libDir = confValues.value("Libraries", {DEFAULT_LIBRARY_DIR}).front();
    if (QMFs::isPathRelative(libDir)) {
        libDir = prefix + "/" + libDir;
    }

    shareDir = confValues.value("Share", {DEFAULT_SHARE_DIR}).front();
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

    QStringList fonts = confValues.value("Fonts", {});
    for (auto path : qAsConst(fonts)) {
        if (QMFs::isPathRelative(path)) {
            path = prefix + "/" + path;
        }
        fontPaths.append(path);
    }

    // Polymorphic factory
    QScopedPointer<QMCoreInitFactory> fac(createFactory());

    // Create instances
    s_dec = fac->createDecorator(q);
    qCDebug(qAppExtLog) << s_dec->metaObject()->className() << "initializing.";

    s_cs = fac->createConsole(q);
    qCDebug(qAppExtLog) << s_cs->metaObject()->className() << "initializing.";

    q->connect(qApp, &QCoreApplication::aboutToQuit, this, [this]() {
        isAboutToQuit = true; //
    });

    // Add translation paths
    for (const auto &path : qAsConst(translationPaths))
        s_dec->addTranslationPath(path);

    // Set default app share dir and app plugins dir
    appShareDir = shareDir
#ifndef Q_OS_MAC
                  + "/" + qApp->applicationName()
#endif
        ;

    appPluginsDir =
#ifdef Q_OS_MAC
        appUpperDir() + "/Plugins"
#else
        libDir + "/" + QCoreApplication::applicationName() + "/plugins"
#endif
        ;
}

QMCoreInitFactory *QMCoreAppExtensionPrivate::createFactory() {
    return new QMCoreInitFactory();
}

QMCoreAppExtension::QMCoreAppExtension(QObject *parent) : QMCoreAppExtension(*new QMCoreAppExtensionPrivate(), parent) {
}

QMCoreAppExtension::~QMCoreAppExtension() {
    destruct();
}

bool QMCoreAppExtension::isAboutToQuit() const {
    Q_D(const QMCoreAppExtension);
    return d->isAboutToQuit;
}

QString QMCoreAppExtension::appDataDir() const {
    Q_D(const QMCoreAppExtension);
    return d->appDataDir;
}

void QMCoreAppExtension::setAppDataDir(const QString &dir) {
    Q_D(QMCoreAppExtension);
    d->appDataDir = dir;
}

QString QMCoreAppExtension::tempDir() const {
    Q_D(const QMCoreAppExtension);
    return d->tempDir;
}

void QMCoreAppExtension::setTempDir(const QString &dir) {
    Q_D(QMCoreAppExtension);
    d->tempDir = dir;
}

QString QMCoreAppExtension::libDir() const {
    Q_D(const QMCoreAppExtension);
    return d->libDir;
}

void QMCoreAppExtension::setLibDir(const QString &dir) {
    Q_D(QMCoreAppExtension);
    d->libDir = dir;
}

QString QMCoreAppExtension::shareDir() const {
    Q_D(const QMCoreAppExtension);
    return d->shareDir;
}

void QMCoreAppExtension::setShareDir(const QString &dir) {
    Q_D(QMCoreAppExtension);
    d->shareDir = dir;
}

QString QMCoreAppExtension::appShareDir() const {
    Q_D(const QMCoreAppExtension);
    return d->appShareDir;
}

void QMCoreAppExtension::setAppShareDir(const QString &dir) {
    Q_D(QMCoreAppExtension);
    d->appShareDir = dir;
}

QString QMCoreAppExtension::appPluginsDir() const {
    Q_D(const QMCoreAppExtension);
    return d->appPluginsDir;
}

void QMCoreAppExtension::setAppPluginsDir(const QString &dir) {
    Q_D(QMCoreAppExtension);
    d->appPluginsDir = dir;
}

bool QMCoreAppExtension::createDataAndTempDirs() const {
    auto func = [](const QString &path) {
        qCDebug(qAppExtLog) << "qmcorehost:" << (QMFs::isDirExist(path) ? "find" : "create") << "directory" << path;
        if (!QMFs::mkDir(path)) {
            qmCon->MsgBox(nullptr, QMCoreConsole::Critical, qApp->applicationName(),
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

QMCoreAppExtension::QMCoreAppExtension(QMCoreAppExtensionPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}