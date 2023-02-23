#ifndef CCONSOLE_P_H
#define CCONSOLE_P_H

#include "../CConsole.h"
#include "private/QsCoreConsole_p.h"

class QMWIDGETS_API CConsolePrivate : public QsCoreConsolePrivate {
    Q_DECLARE_PUBLIC(CConsole)
public:
    CConsolePrivate();
    ~CConsolePrivate();

    void init();
};

#endif // CCONSOLE_P_H
