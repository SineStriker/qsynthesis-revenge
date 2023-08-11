//
// Created by Crs_1 on 2023/8/11.
//

#include "QSystemSemaphoreExtended_p.h"

#ifdef Q_OS_WIN

#include <qt_windows.h>

bool QSystemSemaphoreExtendedPrivate::tryAcquire(int timeout) {
    if(handle() == NULL) return false;
    if(::WaitForSingleObjectEx(semaphore, timeout, FALSE) == WAIT_OBJECT_0) {
        return true;
    } else {
        return false;
    }
}
int QSystemSemaphoreExtendedPrivate::available() {
    long ret;
    ::ReleaseSemaphore(semaphore, 0, &ret);
    return ret;
}

#endif // Q_OS_WIN
