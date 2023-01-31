#ifndef CAPPLICATIONPRIVATE_H
#define CAPPLICATIONPRIVATE_H

#include "../CApplication.h"
#include "CDecorator.h"
#include "Tools/LocalLinguist.h"

#include "QsFrameworkGlobal.h"

class SingleApplication;

class QSFRAMEWORK_API CApplicationPrivate {
    Q_DECLARE_PUBLIC(CApplication)
public:
    CApplicationPrivate();
    virtual ~CApplicationPrivate() = default;

    void init();
    void deinit();

    CApplication *q_ptr;

    CDecorator *hDecorator;
    SingleApplication *hSingleApp;

private:
    LocalLinguist *ll;
};

#endif // CAPPLICATIONPRIVATE_H
