#include "CConsole.h"
#include "private/CConsole_p.h"

#include <QMessageBox>

CConsole::CConsole(QObject *parent) : CConsole(*new CConsolePrivate(), parent) {
}

CConsole::~CConsole() {
}

void CConsole::MsgBox(QObject *parent, QsCoreConsole::MessageBoxFlag flag, const QString &title,
                      const QString &text) {
    Q_D(CConsole);

    QWidget *w = nullptr;
    if (parent && parent->isWidgetType()) {
        w = qobject_cast<QWidget *>(parent);
    }

#if defined(Q_OS_WINDOWS)
    d->osMessageBox_helper(w ? (HWND) w->winId() : nullptr, flag, title, text);
#elif defined(Q_OS_MAC)
    d->osMessageBox_helper(nullptr, flag, title, text);
#else
    switch (flag) {
        case Critical:
            QMessageBox::critical(w, title, text);
            break;
        case Warning:
            QMessageBox::warning(w, title, text);
            break;
        case Question:
            QMessageBox::question(w, title, text);
            break;
        case Information:
            QMessageBox::information(w, title, text);
            break;
    };
#endif
}

CConsole::CConsole(CConsolePrivate &d, QObject *parent) : QsCoreConsole(d, parent) {
    d.init();
}
