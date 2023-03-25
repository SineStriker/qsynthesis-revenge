#include "QMCoreHost_p.h"

#include "QMSystem.h"
#include "QSettingsJsonFormat.h"
#include "private/QMNamespace_p.h"

#include <QCoreApplication>
#include <QDebug>

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

    QDir binDir(qApp->applicationDirPath());
    binDir.cdUp();

    libDir = binDir.absoluteFilePath("lib");
    shareDir = binDir.absoluteFilePath("share");

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