#ifndef QMCONSOLE_P_H
#define QMCONSOLE_P_H

#include "../QMConsole.h"
#include "private/QMCoreConsole_p.h"

class QMWIDGETS_API QMConsolePrivate : public QMCoreConsolePrivate {
    Q_DECLARE_PUBLIC(QMConsole)
public:
    QMConsolePrivate();
    ~QMConsolePrivate();

    void init();
};

#endif // QMCONSOLE_P_H
