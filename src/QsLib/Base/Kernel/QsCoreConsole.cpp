#include "QsCoreConsole.h"
#include "private/QsCoreConsole_p.h"

#include "QsSystem.h"

Q_SINGLETON_DECLARE(QsCoreConsole);

QsCoreConsole::QsCoreConsole(QObject *parent) : QsCoreConsole(*new QsCoreConsolePrivate(), parent) {
}

QsCoreConsole::~QsCoreConsole() {
}

void QsCoreConsole::MsgBox(QObject *parent, QsCoreConsole::MessageBoxFlag flag,
                           const QString &title, const QString &text) {
    Q_UNUSED(parent);
#ifdef Q_OS_WINDOWS
    Q_D(QsCoreConsole);
    d->windowsMessageBox_helper(nullptr, flag, title, text);
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

void QsCoreConsole::SelectBox(QObject *parent, bool supportPreview, int max, const QString &title,
                              const QString &caption,
                              const QList<QsCoreConsole::SelectOption> &arguments) {
}

QsCoreConsole::QsCoreConsole(QsCoreConsolePrivate &d, QObject *parent)
    : QObject(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}
