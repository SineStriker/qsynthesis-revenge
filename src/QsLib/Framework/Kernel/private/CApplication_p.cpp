#include "CApplication_p.h"

#include "../CStartupInfo.h"

#include "QsSystem.h"

#include "Global/QsNamespace.h"

#include "private/QMetaTypeImpl_p.h"
#include "private/QsNamespace_p.h"

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif

#include <SingleApplication>

#include <QMessageBox>
#include <QScreen>

static const SingleApplication::Options opts = SingleApplication::ExcludeAppPath |
                                               SingleApplication::ExcludeAppVersion |
                                               SingleApplication::SecondaryNotification;

CApplicationPrivate::CApplicationPrivate() {
}

void CApplicationPrivate::init() {
    Q_Q(CApplication);

    // Maybe no return
    // ...

    Register_QsNamespace();
    Register_QMetaTypeImpl();

    hDecorator = new QsDecorator(q);

    // Complete other registration
    ll = new LocalLinguist(q);
    ll->addLocale(QsDecorator::UnitedStates, {});
    ll->addLocale(QsDecorator::China, {":/translations/QsFramework_zh_CN.qm"});
    ll->addLocale(QsDecorator::HongKong, {":/translations/QsFramework_zh_HK.qm"});
    ll->addLocale(QsDecorator::Japan, {":/translations/QsFramework_ja_JP.qm"});
    ll->reloadStrings(qIDec->locale());

    // Analyze startup info
    Q_ASSERT(qIStup);

    auto &parser = qIStup->parser;
    parser.setApplicationDescription(qIStup->appDescription);

    QCommandLineOption option_allowRoot("allow-root", "Allow running with super user privileges.");
    parser.addOption(option_allowRoot);

    q->setApplicationName(qIStup->appName);
    q->setApplicationVersion(qIStup->appVersion);
    q->setApplicationDisplayName(qIStup->appDisplayName);

    parser.process(*q);

    // Init Privilege Detections
    if (QsSys::isUserRoot()) {
        if (!parser.isSet(option_allowRoot)) {
            QString title = q->mainTitle();
            QString msg = CApplication::tr("You're trying to start %1 as the %2, which may cause "
                                           "security problem and isn't recommended.")
                              .arg(qAppName(), qApp->sysRootUserName());
            QsSys::osMessageStderr(title, msg);
            QsSys::exitApp(0);
        }
    }

    hSingleApp = new SingleApplication(q, true, opts);

    // Init Single Structures
    if (!qIStup->allowSecondary) {
        if (!hSingleApp->isPrimary()) {
            qInfo() << "Primary instance already running. PID:" << hSingleApp->primaryPid();

            // This eventually needs moved into the NotepadNextApplication to keep
            // sending/receiving logic in the same place
            QByteArray buffer;
            QDataStream stream(&buffer, QIODevice::WriteOnly);

            stream << q->arguments();
            hSingleApp->sendMessage(buffer);

            qInfo() << "Secondary instance closing...";

            q->exit(0);

            ::exit(0);
        }

        qInfo() << "Primary instance initializing...";
    }

    q->connect(hSingleApp, &SingleApplication::instanceStarted, q,
               &CApplication::_q_instanceStarted);
    q->connect(hSingleApp, &SingleApplication::receivedMessage, q,
               &CApplication::_q_messageReceived);
}

void CApplicationPrivate::deinit() {
    delete hSingleApp;
    delete hDecorator;
}
