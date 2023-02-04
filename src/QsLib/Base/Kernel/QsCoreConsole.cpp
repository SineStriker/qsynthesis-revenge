#include "QsCoreConsole.h"
#include "private/QsCoreConsole_p.h"

#include "QsSystem.h"

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif

Q_SINGLETON_DECLARE(QsCoreConsole);

QsCoreConsole::QsCoreConsole(QObject *parent) : QsCoreConsole(*new QsCoreConsolePrivate(), parent) {
}

QsCoreConsole::~QsCoreConsole() {
}

void QsCoreConsole::MsgBox(QObject *parent, QsCoreConsole::MessageBoxFlag flag,
                           const QString &title, const QString &text) {
    Q_UNUSED(parent);
#ifdef Q_OS_WINDOWS
    int winFlag;
    switch (flag) {
        case Critical:
            winFlag = MB_ICONERROR;
            break;
        case Warning:
            winFlag = MB_ICONWARNING;
            break;
        case Question:
            winFlag = MB_ICONQUESTION;
            break;
        case Information:
            winFlag = MB_ICONINFORMATION;
            break;
    };

    ::MessageBoxW(0, text.toStdWString().data(), title.toStdWString().data(),
                  MB_OK | MB_TOPMOST | MB_SETFOREGROUND | winFlag);
#else
    switch (flag) {
        case Critical:
        case Warning:
            fputs(qPrintable(msg), stderr);
            break;
        case Question:
        case Information:
            fputs(qPrintable(msg), stdout);
            break;
    };
#endif
}

QsCoreConsole::QsCoreConsole(QsCoreConsolePrivate &d, QObject *parent)
    : QObject(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}
