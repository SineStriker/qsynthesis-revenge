#ifndef EXTENTMANAGERPRIVATE_H
#define EXTENTMANAGERPRIVATE_H

#include "Vendor/Objects/BasicManager_p.h"

#include "ExtentManager.h"

class ExtentManagerPrivate : public BasicManagerPrivate {
    Q_DECLARE_PUBLIC(ExtentManager)
public:
    ExtentManagerPrivate();
    ~ExtentManagerPrivate();

    void init();
};

#endif // EXTENTMANAGERPRIVATE_H
