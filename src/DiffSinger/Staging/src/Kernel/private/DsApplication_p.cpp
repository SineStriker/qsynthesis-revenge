#include "DsApplication_p.h"

// #include "Kernel/Events.h"

#include "SystemHelper.h"
#include "ViewHelper.h"

#include <QMessageBox>

DsApplicationPrivate::DsApplicationPrivate() {
}

DsApplicationPrivate::~DsApplicationPrivate() {
}

void DsApplicationPrivate::init() {
    Q_Q(DsApplication);

    // Register user types
    // QEventImpl::Register();

    // windowMgr = new WindowManager(q);

    ll = new LocalLinguist(q);
    ld = new LocalDecorator(q);

    Q_TR_NOTIFY_PRIVATE(DsApplication);
    Q_SS_NOTIFY_PRIVATE(DsApplication);
}

void DsApplicationPrivate::deinit() {
    // delete windowMgr;
}
