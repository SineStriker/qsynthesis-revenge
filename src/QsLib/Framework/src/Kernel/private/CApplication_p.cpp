#include "CApplication_p.h"
#include "QMetaTypeImpl.h"

#include "Kernel/MyStartupInfo.h"
#include "SystemHelper.h"

#include "Kernel/QsNamespace.h"

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif

#include <SingleApplication>

#include <QMessageBox>
#include <QScreen>

[[maybe_unused]] static void _str_1() {
    static QString MyCommandLineParser_strings[] = {
        MyCommandLineParser::tr("Display version information."),
        MyCommandLineParser::tr("Display help on commandline options."),
        MyCommandLineParser::tr("Display help including Qt specific options."),
        MyCommandLineParser::tr("Unknown option '%1'."),
        MyCommandLineParser::tr("Unknown options: %1."),
        MyCommandLineParser::tr("Missing value after '%1'."),
        MyCommandLineParser::tr("Unexpected value after '%1'."),
        MyCommandLineParser::tr("[options]"),
        MyCommandLineParser::tr("Usage: %1"),
        MyCommandLineParser::tr("Options:"),
        MyCommandLineParser::tr("Arguments:"),
        MyCommandLineParser::tr("Display Qt version."),
        MyCommandLineParser::tr("Qt version %1\n"),
    };
}

static const SingleApplication::Options opts = SingleApplication::ExcludeAppPath |
                                               SingleApplication::ExcludeAppVersion |
                                               SingleApplication::SecondaryNotification;

CApplicationPrivate::CApplicationPrivate() {
}

void CApplicationPrivate::init() {
    Q_Q(CApplication);

    // Maybe no return
    // ...

    QMetaTypeImpl::Register();
    Qs::Register();

    ll = new LocalLinguist(q);
    ld = new LocalDecorator(q);

    hModes = new MultistyleHandle(q);

    Q_TR_NOTIFY_PRIVATE(CApplication);
    Q_SS_NOTIFY_PRIVATE(CApplication);

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
    if (Sys::isUserRoot()) {
        QString title = q->mainTitle();
        QString msg = CApplication::tr("You're trying to start %1 as the %2, which may cause "
                                       "security problem and isn't recommended.")
                          .arg(qAppName(), qApp->sysRootUserName());
        if (!parser.isSet(option_allowRoot)) {
#ifdef Q_OS_WINDOWS
            ::MessageBoxW(0, msg.toStdWString().data(), title.toStdWString().data(),
                          MB_OK | MB_TOPMOST | MB_SETFOREGROUND | MB_ICONWARNING);
#elif Q_OS_LINUX
            fputs(qPrintable(msg), stdout);
#else
            QMessageBox::warning(nullptr, title, msg, CApplication::tr("Confirm"));
#endif
            Sys::exitApp(0);
        }
    }

    hSingle = new SingleApplication(q, true, opts);

    // Init Single Structures
    if (!qIStup->allowSecondary) {
        if (!hSingle->isPrimary()) {
            qInfo() << "Primary instance already running. PID:" << hSingle->primaryPid();

            // This eventually needs moved into the NotepadNextApplication to keep
            // sending/receiving logic in the same place
            QByteArray buffer;
            QDataStream stream(&buffer, QIODevice::WriteOnly);

            stream << q->arguments();
            hSingle->sendMessage(buffer);

            qInfo() << "Secondary instance closing...";

            q->exit(0);

            ::exit(0);
        }

        qInfo() << "Primary instance initializing...";
    }

    q->connect(hSingle, &SingleApplication::instanceStarted, q, &CApplication::_q_instanceStarted);
    q->connect(hSingle, &SingleApplication::receivedMessage, q, &CApplication::_q_messageReceived);
    q->connect(q->primaryScreen(), &QScreen::logicalDotsPerInchChanged, q,
               &CApplication::_q_screenRatioChanged);
}

void CApplicationPrivate::deinit() {
    delete hSingle;
    delete hModes;
}
