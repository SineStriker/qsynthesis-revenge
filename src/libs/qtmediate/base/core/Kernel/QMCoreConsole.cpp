#include "QMCoreConsole.h"
#include "private/QMCoreConsole_p.h"

#include "choruskit_config.h"

Q_SINGLETON_DECLARE(QMCoreConsole);

#ifdef Q_OS_WINDOWS
#    include <Windows.h>
#elif defined(Q_OS_MACOS)
#include <CoreFoundation/CoreFoundation.h>
#endif

QMCoreConsolePrivate::QMCoreConsolePrivate() {
}

QMCoreConsolePrivate::~QMCoreConsolePrivate() {
}

void QMCoreConsolePrivate::init() {
}

#if defined(Q_OS_WINDOWS) || defined(Q_OS_MAC)

void QMCoreConsolePrivate::osMessageBox_helper(void *winHandle, QMCoreConsole::MessageBoxFlag flag,
                                               const QString &title, const QString &text) {
#    ifdef Q_OS_WINDOWS
    int winFlag;
    switch (flag) {
        case QMCoreConsole::Critical:
            winFlag = MB_ICONERROR;
            break;
        case QMCoreConsole::Warning:
            winFlag = MB_ICONWARNING;
            break;
        case QMCoreConsole::Question:
            winFlag = MB_ICONQUESTION;
            break;
        case QMCoreConsole::Information:
            winFlag = MB_ICONINFORMATION;
            break;
    };

    ::MessageBoxW(static_cast<HWND>(winHandle), text.toStdWString().data(), title.toStdWString().data(),
                  MB_OK
#        ifdef CONFIG_WIN32_MSGBOX_TOPMOST
                      | MB_TOPMOST
#        endif
                      | MB_SETFOREGROUND | winFlag);
#else
    // From https://web.archive.org/web/20111127025605/http://jorgearimany.blogspot.com/2010/05/messagebox-from-windows-to-mac.html
    CFOptionFlags result;
    int level = 0;
    switch (flag) {
        case QMCoreConsole::Critical:
            level = 2;
            break;
        case QMCoreConsole::Warning:
            level = 1;
            break;
        case QMCoreConsole::Question:
            level = 3;
            break;
        case QMCoreConsole::Information:
            level = 0;
            break;
    };
    CFUserNotificationDisplayAlert(
        0, // no timeout
        level, //change it depending message_type flags ( MB_ICONASTERISK.... etc.)
        NULL, //icon url, use default, you can change it depending message_type flags
        NULL, //not used
        NULL, //localization of strings
        title.toCFString(), //header text
        text.toCFString(), //message text
        NULL, //default "ok" text in button
        NULL, //alternate button title
        NULL, //other button title, null--> no other button
        &result //response flags
    );
#endif
}

#endif

QMCoreConsole::QMCoreConsole(QObject *parent) : QMCoreConsole(*new QMCoreConsolePrivate(), parent) {
}

QMCoreConsole::~QMCoreConsole() {
    destruct();
}

void QMCoreConsole::MsgBox(QObject *parent, QMCoreConsole::MessageBoxFlag flag, const QString &title,
                           const QString &text) {
    Q_UNUSED(parent);

#if defined(Q_OS_WINDOWS) || defined(Q_OS_MAC)
    Q_D(QMCoreConsole);
    d->osMessageBox_helper(nullptr, flag, title, text);
#else
    switch (flag) {
        case Critical:
        case Warning:
            fputs(qPrintable(text), stderr);
            break;
        case Question:
        case Information:
            fputs(qPrintable(text), stdout);
            break;
    };
#endif
}

QMCoreConsole::QMCoreConsole(QMCoreConsolePrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}
