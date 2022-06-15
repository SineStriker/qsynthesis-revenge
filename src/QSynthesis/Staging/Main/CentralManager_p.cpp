#include "CentralManager_p.h"

#include "MainWindow.h"

CentralManagerPrivate::CentralManagerPrivate() {
}

CentralManagerPrivate::~CentralManagerPrivate() {
}

void CentralManagerPrivate::init() {
    Q_Q(CentralManager);

    w = qobject_cast<MainWindow *>(q->parent());
    Q_ASSERT(w);

    w->installEventFilter(q);
}
