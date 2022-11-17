#ifndef LVAPPLICATIONPRIVATE_H
#define LVAPPLICATIONPRIVATE_H

#include "../LvApplication.h"

#include "Kernel/private/LvElemApplication_p.h"

#include "Managers/WindowManager.h"

class LvApplicationPrivate : public LvElemApplicationPrivate {
    Q_DECLARE_PUBLIC(LvApplication)
public:
    LvApplicationPrivate();
    ~LvApplicationPrivate();

    void init();
    void deinit();

    WindowManager *windowMgr;

    void messageReceived_helper(const QStringList &args) override;
};

#endif // LVAPPLICATIONPRIVATE_H
