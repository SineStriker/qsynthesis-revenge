#ifndef QSCORECONSOLE_P_H
#define QSCORECONSOLE_P_H

#include "../QsCoreConsole.h"

class QMCORELIB_API QsCoreConsolePrivate {
    Q_DECLARE_PUBLIC(QsCoreConsole)
public:
    QsCoreConsolePrivate();
    virtual ~QsCoreConsolePrivate();

    void init();

#if defined(Q_OS_WINDOWS) || defined(Q_OS_MAC)
    void osMessageBox_helper(void *winHandle, QsCoreConsole::MessageBoxFlag flag,
                             const QString &title, const QString &text);
#endif

    QsCoreConsole *q_ptr;
};

#endif // QSCORECONSOLE_P_H
