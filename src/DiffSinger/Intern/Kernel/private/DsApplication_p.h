#ifndef DSAPPLICATIONPRIVATE_H
#define DSAPPLICATIONPRIVATE_H

#include "../DsApplication.h"

#include "Kernel/private/QsApplication_p.h"
#include "Managers/WindowManager.h"

class DsApplicationPrivate : public QsApplicationPrivate {
    Q_DECLARE_PUBLIC(DsApplication)
public:
    DsApplicationPrivate();
    ~DsApplicationPrivate();

    void init();
    void deinit();

    WindowManager *windowMgr;

private:
    CDecreateDir dd;
};

#endif // DSAPPLICATIONPRIVATE_H
