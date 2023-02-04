#ifndef QSCORECONSOLEPRIVATE_H
#define QSCORECONSOLEPRIVATE_H

#include "../QsCoreConsole.h"

class QSBASE_API QsCoreConsolePrivate {
    Q_DECLARE_PUBLIC(QsCoreConsole)
public:
    QsCoreConsolePrivate();
    virtual ~QsCoreConsolePrivate();

    void init();

    QsCoreConsole *q_ptr;
};

#endif // QSCORECONSOLEPRIVATE_H
