#ifndef QSCONSOLEPRIVATE_H
#define QSCONSOLEPRIVATE_H

#include "../QsConsole.h"
#include "private/CConsole_p.h"

class QSINTEGRATE_API QsConsolePrivate : public CConsolePrivate {
    Q_DECLARE_PUBLIC(QsConsole)
public:
    QsConsolePrivate();
    ~QsConsolePrivate();

    void init();
};

#endif // QSCONSOLEPRIVATE_H
