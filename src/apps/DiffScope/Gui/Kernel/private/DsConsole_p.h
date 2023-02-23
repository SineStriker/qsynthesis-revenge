#ifndef DSCONSOLEPRIVATE_H
#define DSCONSOLEPRIVATE_H

#include "private/QsConsole_p.h"

#include "../DsConsole.h"

class DsConsolePrivate : public QsConsolePrivate {
    Q_DECLARE_PUBLIC(DsConsole)
public:
    DsConsolePrivate();
    ~DsConsolePrivate();

    void init();
};

#endif // DSCONSOLEPRIVATE_H
