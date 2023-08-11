//
// Created by Crs_1 on 2023/8/11.
//

#include "QSystemSemaphoreExtended_p.h"

#ifdef Q_OS_UNIX

#include <ctime>
#include <private/qcore_unix_p.h>

bool QSystemSemaphoreExtendedPrivate::tryAcquire(int timeout) {
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_op = -1;
    operation.sem_flg = SEM_UNDO;

    struct timespec ts;
    ts.tv_sec = timeout / 1000;
    ts.tv_nsec = (timeout % 1000) * 1000 * 1000;

    executeAgain:
    if (-1 == handle())
        return false;

    int res;
    EINTR_LOOP(res, semtimedop(semaphore, &operation, 1, &ts));
    if (-1 == res) {
        if (errno == EINVAL || errno == EIDRM) {
            semaphore = -1;
            cleanHandle();
            handle();
            goto executeAgain;
        }
        return false;
    }

    clearError();
    return true;
}

#endif // Q_OS_UNIX