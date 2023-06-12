#include "QMConsole.h"
#include "private/QMConsole_p.h"

#include <QMessageBox>

QMConsolePrivate::QMConsolePrivate() {
}

QMConsolePrivate::~QMConsolePrivate() {
}

void QMConsolePrivate::init() {
}

QMConsole::QMConsole(QObject *parent) : QMConsole(*new QMConsolePrivate(), parent) {
}

QMConsole::~QMConsole() {
}

void QMConsole::MsgBox(QObject *parent, QMCoreConsole::MessageBoxFlag flag, const QString &title,
                      const QString &text) {
    Q_D(QMConsole);

    QWidget *w = nullptr;
    if (parent && parent->isWidgetType()) {
        w = qobject_cast<QWidget *>(parent)->window();
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

QMConsole::QMConsole(QMConsolePrivate &d, QObject *parent) : QMGuiConsole(d, parent) {
    d.init();
}
