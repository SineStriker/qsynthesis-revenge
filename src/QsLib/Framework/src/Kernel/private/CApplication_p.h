#ifndef CAPPLICATIONPRIVATE_H
#define CAPPLICATIONPRIVATE_H

#include "../CApplication.h"
#include "../MultistyleHandle.h"

#include "Kernel/LocalDecorator.h"
#include "Kernel/LocalLinguist.h"

#include "qsframework_global.h"

class SingleApplication;

class QSFRAMEWORK_API CApplicationPrivate {
    Q_DECLARE_PUBLIC(CApplication)
public:
    CApplicationPrivate();
    virtual ~CApplicationPrivate() = default;

    void init();
    void deinit();

    CApplication *q_ptr;

    MultistyleHandle *hModes;
    SingleApplication *hSingle;

private:
    LocalLinguist *ll;
    LocalDecorator *ld;
};

#endif // CAPPLICATIONPRIVATE_H
