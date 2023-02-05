#ifndef DSCONSOLEPRIVATE_H
#define DSCONSOLEPRIVATE_H

#include "private/CConsole_p.h"

#include "../DsConsole.h"

class DsConsolePrivate : public CConsolePrivate {
    Q_DECLARE_PUBLIC(DsConsole)
public:
    DsConsolePrivate();
    ~DsConsolePrivate();

    void init();

    QString fileDlgFilter_project;
    QString fileDlgFilter_import;
};

#endif // DSCONSOLEPRIVATE_H
