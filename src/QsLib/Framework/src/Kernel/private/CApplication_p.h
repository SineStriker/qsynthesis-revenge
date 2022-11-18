#ifndef CAPPLICATIONPRIVATE_H
#define CAPPLICATIONPRIVATE_H

#include "../CApplication.h"

#include <SingleApplication>

#include "../MultistyleHandle.h"

#include "qsframework_global.h"

class QSFRAMEWORK_API CApplicationPrivate {
    Q_DECLARE_PUBLIC(CApplication)
public:
    CApplicationPrivate();
    virtual ~CApplicationPrivate() = default;

    void init();
    void deinit();

    void setupSingle();

    virtual void instanceStarted_helper();
    virtual void messageReceived_helper(const QStringList &args);

    CApplication *q_ptr;

    QScopedPointer<SingleApplication> hSingle;
    QScopedPointer<MultistyleHandle> hMSH;

    QList<CAppNotifyFilter *> notifyFilters;
};

#endif // CAPPLICATIONPRIVATE_H
