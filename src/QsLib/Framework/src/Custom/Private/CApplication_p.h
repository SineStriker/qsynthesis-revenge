#ifndef CAPPLICATIONPRIVATE_H
#define CAPPLICATIONPRIVATE_H

#include "../CApplication.h"

#include "qsframework_global.h"

class QSFRAMEWORK_API CApplicationPrivate {
    Q_DECLARE_PUBLIC(CApplication)
public:
    CApplicationPrivate();
    virtual ~CApplicationPrivate() = default;

    void init();

    CApplication *q_ptr;

    QList<CAppNotifyFilter *> notifyFilters;
};

#endif // CAPPLICATIONPRIVATE_H
