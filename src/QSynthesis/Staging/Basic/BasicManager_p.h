#ifndef BASICMANAGERPRIVATE_H
#define BASICMANAGERPRIVATE_H

#include "BasicManager.h"

class BasicManagerPrivate {
    Q_DECLARE_PUBLIC(BasicManager)
public:
    BasicManagerPrivate();
    virtual ~BasicManagerPrivate();

    void init();

    BasicManager *q_ptr;
};

#endif // BASICMANAGERPRIVATE_H