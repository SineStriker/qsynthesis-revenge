#ifndef SINGLEAPPHANDLEPRIVATE_H
#define SINGLEAPPHANDLEPRIVATE_H

#include "../SingleAppHandle.h"

class SingleAppHandlePrivate {
    Q_DECLARE_PUBLIC(SingleAppHandle)
public:
    SingleAppHandlePrivate();
    virtual ~SingleAppHandlePrivate();

    void init();

    SingleAppHandle *q_ptr;

    void setup_helper();
};

#endif // SINGLEAPPHANDLEPRIVATE_H
