#ifndef CCONSOLEPRIVATE_H
#define CCONSOLEPRIVATE_H

#include "../CConsole.h"
#include "private/QsCoreConsole_p.h"

class CConsolePrivate : public QsCoreConsolePrivate {
    Q_DECLARE_PUBLIC(CConsole)
public:
    CConsolePrivate();
    ~CConsolePrivate();

    void init();
};

#endif // CCONSOLEPRIVATE_H
