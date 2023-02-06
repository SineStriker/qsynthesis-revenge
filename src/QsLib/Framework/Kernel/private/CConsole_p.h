#ifndef CCONSOLEPRIVATE_H
#define CCONSOLEPRIVATE_H

#include "../CConsole.h"
#include "private/QsCoreConsole_p.h"

class QSFRAMEWORK_API CConsolePrivate : public QsCoreConsolePrivate {
    Q_DECLARE_PUBLIC(CConsole)
public:
    CConsolePrivate();
    ~CConsolePrivate();

    void init();
};

#endif // CCONSOLEPRIVATE_H
