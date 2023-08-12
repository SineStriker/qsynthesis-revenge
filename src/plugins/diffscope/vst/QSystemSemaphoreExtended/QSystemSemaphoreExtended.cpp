//
// Created by Crs_1 on 2023/8/11.
//

#include "QSystemSemaphoreExtended.h"
#include "QSystemSemaphoreExtended_p.h"

QSystemSemaphoreExtended::QSystemSemaphoreExtended(const QString &key, int initialValue,
                                                   QSystemSemaphore::AccessMode mode)
    : QSystemSemaphore(key, initialValue, mode) {

}
QSystemSemaphoreExtended::~QSystemSemaphoreExtended() {
}

bool QSystemSemaphoreExtended::tryAcquire(int timeout) {
    Q_D(QSystemSemaphoreExtended);
    return d->tryAcquire(timeout);
}

QSystemSemaphoreExtendedPrivate::QSystemSemaphoreExtendedPrivate(): QSystemSemaphorePrivate() {
}