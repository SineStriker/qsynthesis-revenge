//
// Created by Crs_1 on 2023/8/11.
//

#ifndef CHORUSKIT_QSYSTEMSEMAPHOREEXTENDED_P_H
#define CHORUSKIT_QSYSTEMSEMAPHOREEXTENDED_P_H

#include "QSystemSemaphoreExtended.h"

#include <private/qsystemsemaphore_p.h>

class QSystemSemaphoreExtendedPrivate: public QSystemSemaphorePrivate {
public:
    QSystemSemaphoreExtendedPrivate();
    bool tryAcquire(int timeout);
};

#endif // CHORUSKIT_QSYSTEMSEMAPHOREEXTENDED_P_H
