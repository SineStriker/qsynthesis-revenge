#include "SingleAppHandle.h"
#include "private/SingleAppHandle_p.h"

SingleAppHandle::SingleAppHandle(QObject *parent)
    : SingleAppHandle(*new SingleAppHandlePrivate(), parent) {
}

SingleAppHandle::~SingleAppHandle() {
}

void SingleAppHandle::setup() {
    Q_D(SingleAppHandle);
    d->setup_helper();
}

SingleAppHandle::SingleAppHandle(SingleAppHandlePrivate &d, QObject *parent)
    : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
