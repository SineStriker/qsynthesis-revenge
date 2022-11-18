#include "LvApplication_p.h"

#include "Kernel/Events.h"

#include "SystemHelper.h"
#include "ViewHelper.h"

#include <QMessageBox>

LvApplicationPrivate::LvApplicationPrivate() {
}

LvApplicationPrivate::~LvApplicationPrivate() {
}

void LvApplicationPrivate::init() {
    Q_Q(LvApplication);

    // Register user types
    QEventImpl::Register();

    windowMgr = new WindowManager(q);

    ll = new LocalLinguist(q);
    ld = new LocalDecorator(q);

    Q_TR_NOTIFY_PRIVATE(LvApplication);
    Q_SS_NOTIFY_PRIVATE(LvApplication);
}

void LvApplicationPrivate::deinit() {
    delete windowMgr;
}
