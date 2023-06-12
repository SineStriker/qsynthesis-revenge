#ifndef QMCORECONSOLE_P_H
#define QMCORECONSOLE_P_H

#include "../QMCoreConsole.h"

class QMCORE_EXPORT QMCoreConsolePrivate {
    Q_DECLARE_PUBLIC(QMCoreConsole)
public:
    QMCoreConsolePrivate();
    virtual ~QMCoreConsolePrivate();

    void init();

#if defined(Q_OS_WINDOWS) || defined(Q_OS_MAC)
    void osMessageBox_helper(void *winHandle, QMCoreConsole::MessageBoxFlag flag,
                             const QString &title, const QString &text);
#endif

    QMCoreConsole *q_ptr;
};

#endif // QMCORECONSOLE_P_H
