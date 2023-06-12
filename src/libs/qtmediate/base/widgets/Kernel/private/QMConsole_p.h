#ifndef QMCONSOLE_P_H
#define QMCONSOLE_P_H

#include "../QMConsole.h"
#include "private/QMGuiConsole_p.h"

class QMConsolePrivate : public QMGuiConsolePrivate {
    Q_DECLARE_PUBLIC(QMConsole)
public:
    QMConsolePrivate();
    ~QMConsolePrivate();

    void init();
};

#endif // QMCONSOLE_P_H
