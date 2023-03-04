#ifndef LVAPPLICATIONPRIVATE_H
#define LVAPPLICATIONPRIVATE_H

#include "../LvApplication.h"

#include "Kernel/private/LvElemApplication_p.h"

#include "Managers/WindowManager.h"

#include "Kernel/LocalDecorator.h"
#include "Kernel/LocalLinguist.h"

class LvApplicationPrivate : public LvElemApplicationPrivate {
    Q_DECLARE_PUBLIC(LvApplication)
public:
    LvApplicationPrivate();
    ~LvApplicationPrivate();

    void init();
    void deinit();

    WindowManager *windowMgr;

private:
    LocalLinguist *ll;
    LocalDecorator *ld;
};

#endif // LVAPPLICATIONPRIVATE_H
