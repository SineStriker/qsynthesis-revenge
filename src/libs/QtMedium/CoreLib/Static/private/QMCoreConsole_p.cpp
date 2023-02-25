#include "QMCoreConsole_p.h"

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#elif defined(Q_OS_MACOS)
#include <CoreFoundation/CoreFoundation.h>
#include <objc/message.h>
#include <objc/runtime.h>
#define cls objc_getClass
#define sel sel_getUid

typedef id (*_object_message_send)(id, SEL, ...);
typedef id (*_class_message_send)(Class, SEL, ...);

#define _msg ((_object_message_send) objc_msgSend)
#define _cls_msg ((_class_message_send) objc_msgSend)

typedef id (*_MethodImp)(id, SEL, ...);
typedef _MethodImp (*_get_method_imp)(Class, SEL);
#define _method ((_get_method_imp) class_getMethodImplementation)
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
#ifdef Q_OS_WINDOWS
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

    ::MessageBoxW(static_cast<HWND>(winHandle), text.toStdWString().data(),
                  title.toStdWString().data(),
                  MB_OK
#ifdef CONFIG_WIN32_MSGBOX_TOPMOST
                      | MB_TOPMOST
#endif
                      | MB_SETFOREGROUND | winFlag);
#else
    Class alert = cls("NSAlert");
    id alertObj = _cls_msg(alert, sel("alloc"));
    alertObj = _msg(alertObj, sel("init"));
    switch (flag) {
        case QMCoreConsole::Critical:
            _msg(alertObj, sel("setAlertStyle:"), 2);
            break;
        case QMCoreConsole::Warning:
            _msg(alertObj, sel("setAlertStyle:"), 1);
            break;
        case QMCoreConsole::Question:
            _msg(alertObj, sel("setAlertStyle:"), 3);
            break;
        case QMCoreConsole::Information:
            _msg(alertObj, sel("setAlertStyle:"), 0);
            break;
    };
    _msg(alertObj, sel("setMessageText:"), text.toCFString());
    _msg(alertObj, sel("setInformativeText:"), title.toCFString());
    _msg(alertObj, sel("runModal"), 0);
#endif
}

#endif
