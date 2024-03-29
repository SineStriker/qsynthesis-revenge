#include "LvApplication_p.h"

#include "Events_p.h"

#include "QMSystem.h"
#include "ViewHelper.h"

#include <QMessageBox>

LvApplicationPrivate::LvApplicationPrivate() {
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); // HiDPI pixmaps
}

LvApplicationPrivate::~LvApplicationPrivate() {
}

void LvApplicationPrivate::init() {
    Q_Q(LvApplication);

    // Register user types
    Register_Events();

    windowMgr = new WindowManager(q);

    ll = new LocalLinguist(q);
    ld = new LocalDecorator(q);

    Q_TR_NOTIFY_PRIVATE(LvApplication);
    Q_SS_NOTIFY_PRIVATE(LvApplication);
}

void LvApplicationPrivate::deinit() {
    delete windowMgr;
}
