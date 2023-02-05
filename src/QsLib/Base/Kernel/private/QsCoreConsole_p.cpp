#include "QsCoreConsole_p.h"

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif

QsCoreConsolePrivate::QsCoreConsolePrivate() {
}

QsCoreConsolePrivate::~QsCoreConsolePrivate() {
}

void QsCoreConsolePrivate::init() {
}

#ifdef Q_OS_WINDOWS

void QsCoreConsolePrivate::windowsMessageBox_helper(void *winHandle,
                                                    QsCoreConsole::MessageBoxFlag flag,
                                                    const QString &title, const QString &text) {
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
}

#endif
