#ifndef CSTARTINFOPRIVATE_H
#define CSTARTINFOPRIVATE_H

#include "../CStartInfo.h"

#include "CDecorateDir.h"

class QSFRAMEWORK_API CStartInfoPrivate {
    Q_DECLARE_PUBLIC(CStartInfo)
public:
    CStartInfoPrivate();
    virtual ~CStartInfoPrivate();

    void init();
    void load_helper();
    void deinit();

    CStartInfo *q_ptr;

private:
    CDecorateDir dd;
};

#endif // CSTARTINFOPRIVATE_H
