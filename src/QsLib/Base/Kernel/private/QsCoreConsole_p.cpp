#include "QsCoreConsole_p.h"

#include "QsFileManager.h"
#include "QsPluginManager.h"

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

QsCoreConsolePrivate::QsCoreConsolePrivate() {
}

QsCoreConsolePrivate::~QsCoreConsolePrivate() {
    deinit();
}

void QsCoreConsolePrivate::init() {
    fileMgr = nullptr;
    pluginMgr = nullptr;
}

void QsCoreConsolePrivate::load_helper() {
    Q_Q(QsCoreConsole);

    fileMgr = q->createFileManager(q);
    pluginMgr = q->createPluginManager(q);

    fileMgr->load();
    pluginMgr->load();
}

void QsCoreConsolePrivate::save_helper() {
    pluginMgr->save();
    fileMgr->save();
}

void QsCoreConsolePrivate::deinit() {
    delete pluginMgr;
    delete fileMgr;
}

#if defined(Q_OS_WINDOWS) || defined(Q_OS_MAC)

void QsCoreConsolePrivate::osMessageBox_helper(void *winHandle, QsCoreConsole::MessageBoxFlag flag,
                                               const QString &title, const QString &text) {
#ifdef Q_OS_WINDOWS
    int winFlag;
    switch (flag) {
        case QsCoreConsole::Critical:
            winFlag = MB_ICONERROR;
            break;
        case QsCoreConsole::Warning:
            winFlag = MB_ICONWARNING;
            break;
        case QsCoreConsole::Question:
            winFlag = MB_ICONQUESTION;
            break;
        case QsCoreConsole::Information:
            winFlag = MB_ICONINFORMATION;
            break;
    };

    ::MessageBoxW(static_cast<HWND>(winHandle), text.toStdWString().data(),
                  title.toStdWString().data(), MB_OK | MB_TOPMOST | MB_SETFOREGROUND | winFlag);
#else
    Class alert = cls("NSAlert");
    id alertObj = _cls_msg(alert, sel("alloc"));
    alertObj = _msg(alertObj, sel("init"));
    switch (flag) {
        case QsCoreConsole::Critical:
            _msg(alertObj, sel("setAlertStyle:"), 2);
            break;
        case QsCoreConsole::Warning:
            _msg(alertObj, sel("setAlertStyle:"), 1);
            break;
        case QsCoreConsole::Question:
            _msg(alertObj, sel("setAlertStyle:"), 3);
            break;
        case QsCoreConsole::Information:
            _msg(alertObj, sel("setAlertStyle:"), 0);
            break;
    };
    _msg(alertObj, sel("setMessageText:"), text.toCFString());
    _msg(alertObj, sel("setInformativeText:"), title.toCFString());
    _msg(alertObj, sel("runModal"), 0);
#endif
}

#endif
