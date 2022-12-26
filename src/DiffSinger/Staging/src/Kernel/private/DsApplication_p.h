#ifndef DSAPPLICATIONPRIVATE_H
#define DSAPPLICATIONPRIVATE_H

#include "../DsApplication.h"

#include "Kernel/private/QsApplication_p.h"
#include "Managers/WindowManager.h"

#include "Kernel/LocalDecorator.h"
#include "Kernel/LocalLinguist.h"

class DsApplicationPrivate : public QsApplicationPrivate {
    Q_DECLARE_PUBLIC(DsApplication)
public:
    DsApplicationPrivate();
    ~DsApplicationPrivate();

    void init();
    void deinit();

    WindowManager *windowMgr;

private:
    LocalLinguist *ll;
    LocalDecorator *ld;
};

#endif // DSAPPLICATIONPRIVATE_H
