#include "LvApplication_p.h"

#include "Kernel/Events.h"

#include "ViewHelper.h"

LvApplicationPrivate::LvApplicationPrivate() {
}

LvApplicationPrivate::~LvApplicationPrivate() {
}

void LvApplicationPrivate::init() {
    Q_Q(LvApplication);

    // Register user types
    QEventImpl::Register();

    windowMgr = new WindowManager(q);
}

void LvApplicationPrivate::deinit() {
    delete windowMgr;
}

void LvApplicationPrivate::messageReceived_helper(const QStringList &args) {
    Q_UNUSED(args)

    View::bringWindowToForeground(windowMgr->firstWindow());
}
