#include "QMCoreConsole.h"
#include "private/QMCoreConsole_p.h"

#include "QMSystem.h"

Q_SINGLETON_DECLARE(QMCoreConsole);

QMCoreConsole::QMCoreConsole(QObject *parent) : QMCoreConsole(*new QMCoreConsolePrivate(), parent) {
}

QMCoreConsole::~QMCoreConsole() {
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

void QMCoreConsole::SelectBox(QObject *parent, bool supportPreview, int max, const QString &title,
                              const QString &caption, const QList<QMCoreConsole::SelectOption> &arguments) {
}

QMCoreConsole::QMCoreConsole(QMCoreConsolePrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}
