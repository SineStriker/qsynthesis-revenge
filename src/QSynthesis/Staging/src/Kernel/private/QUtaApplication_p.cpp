#include "QUtaApplication_p.h"

// #include "Kernel/Events.h"

#include "SystemHelper.h"
#include "ViewHelper.h"

#include <QMessageBox>

QUtaApplicationPrivate::QUtaApplicationPrivate() {
}

QUtaApplicationPrivate::~QUtaApplicationPrivate() {
}

void QUtaApplicationPrivate::init() {
    Q_Q(QUtaApplication);

    // Register user types
    // QEventImpl::Register();

    // windowMgr = new WindowManager(q);

    ll = new LocalLinguist(q);
    ld = new LocalDecorator(q);

    Q_TR_NOTIFY_PRIVATE(QUtaApplication);
    Q_SS_NOTIFY_PRIVATE(QUtaApplication);
}

void QUtaApplicationPrivate::deinit() {
    // delete windowMgr;
}
