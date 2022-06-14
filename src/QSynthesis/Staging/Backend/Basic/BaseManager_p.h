#ifndef BASEMANAGERPRIVATE_H
#define BASEMANAGERPRIVATE_H

#include "BaseManager.h"

class BaseManagerPrivate {
    Q_DECLARE_PUBLIC(BaseManager)
public:
    BaseManagerPrivate();
    virtual ~BaseManagerPrivate();

    void init();

    BaseManager *q_ptr;
};

#endif // BASEMANAGERPRIVATE_H
