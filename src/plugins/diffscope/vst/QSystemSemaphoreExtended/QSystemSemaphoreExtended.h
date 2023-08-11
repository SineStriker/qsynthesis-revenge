//
// Created by Crs_1 on 2023/8/11.
//

#ifndef CHORUSKIT_QSYSTEMSEMAPHOREEXTENDED_H
#define CHORUSKIT_QSYSTEMSEMAPHOREEXTENDED_H

#include "qsystemsemaphore_friended.h"
#include <QSystemSemaphoreExtendedGlobal.h>

class QSystemSemaphoreExtendedPrivate;

class QSYSTEMSEMAPHOREEXTENDED_EXPORT QSystemSemaphoreExtended: public QSystemSemaphore {
    Q_DECLARE_PRIVATE(QSystemSemaphoreExtended)
public:
    QSystemSemaphoreExtended(const QString &key, int initialValue = 0, AccessMode mode = Open);
    ~QSystemSemaphoreExtended();

    bool tryAcquire(int timeout = 0);
};



#endif // CHORUSKIT_QSYSTEMSEMAPHOREEXTENDED_H
