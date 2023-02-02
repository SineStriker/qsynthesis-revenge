#include "QsCoreStartInfo_p.h"

#include "QsCoreConfig_p.h"
#include "QsSystem.h"
#include "private/QsNamespace_p.h"

#include "../QsCoreDecorator.h"

#include <QDataStream>
#include <QDir>

static const SingleApplication::Options opts = SingleApplication::ExcludeAppPath |
                                               SingleApplication::ExcludeAppVersion |
                                               SingleApplication::SecondaryNotification;

static const char Slash = '/';

static QString GetAppConfig() {
    return qApp->applicationName() + "_settings.json";
}

void QsCoreStartInfoPrivate::init() {
    Q_Q(QsCoreStartInfo);

    dec = nullptr;
    coreConfig = nullptr;
    hSingleApp = nullptr;

    isAboutToQuit = false;

    Register_QsNamespace();

    q->connect(qApp, &QCoreApplication::aboutToQuit, q, &QsCoreStartInfo::_q_aboutToQuit);
}

void QsCoreStartInfoPrivate::load_helper() {
    Q_Q(QsCoreStartInfo);

    // Maybe no return
    // ...

    // Create global decrator
    dec = q->createDecorator(q);
    dec->addLocale("English Default", {{"en_US", {}}});

    // Load or generate distconfig
    QString configPath = qApp->applicationDirPath() + Slash + GetAppConfig();
    coreConfig = q->creatDistConfig();
    coreConfig->d_func()->initAll();
    coreConfig->load(configPath);

    // Install QsBase translation
    ld.setDir(coreConfig->appDir(QsCoreConfig::AppShare));
    ld.loadDefault("QsBase");

    // Parse commandline arguments
    auto &parser = q->parser;
    parser.setApplicationDescription(q->appDescription);

    QCommandLineOption option_allowRoot("allow-root", "Allow running with super user privileges.");
    parser.addOption(option_allowRoot);

    QCommandLineOption option_resetConfig("reset-config", "Reset or generate app config file.");
    parser.addOption(option_resetConfig);

    parser.process(*qApp);

    // Root privilege detection
    if (QsOs::isUserRoot() && !parser.isSet(option_allowRoot)) {
        QString msg = QCoreApplication::tr("You're trying to start %1 as the %2, which may cause "
                                           "security problem and isn't recommended.")
                          .arg(qAppName(), QsOs::rootUserName());
        QsOs::messageStderr(qIStup->mainTitle(), msg);
        QsFs::exitApp(0);
    }

    // Reset configuration, exit
    if (parser.isSet(option_resetConfig)) {
        coreConfig->saveDefault(configPath);
        QsFs::exitApp(0);
    }

    // Init Single Structures
    hSingleApp = new SingleApplication(qApp, true, opts);

    if (!q->allowSecondary) {
        if (!hSingleApp->isPrimary()) {
            qInfo() << "Primary instance already running. PID:" << hSingleApp->primaryPid();

            // This eventually needs moved into the NotepadNextApplication to keep
            // sending/receiving logic in the same place
            QByteArray buffer;
            QDataStream stream(&buffer, QIODevice::WriteOnly);

            stream << qApp->arguments();
            hSingleApp->sendMessage(buffer);

            qInfo() << "Secondary instance closing...";

            qApp->exit(0);

            ::exit(0);
        }

        qInfo() << "Primary instance initializing...";
    }

    // Initialize app data and temp dirs
    if (!coreConfig->apply()) {
        ::exit(-1);
    }
}
