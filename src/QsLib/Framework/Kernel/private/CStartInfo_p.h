#ifndef CSTARTINFOPRIVATE_H
#define CSTARTINFOPRIVATE_H

#include "../CStartInfo.h"

class QSFRAMEWORK_API CStartInfoPrivate {
    Q_DECLARE_PUBLIC(CStartInfo)
public:
    CStartInfoPrivate();
    virtual ~CStartInfoPrivate();

    void init();

    CStartInfo *q_ptr;
};

#endif // CSTARTINFOPRIVATE_H
