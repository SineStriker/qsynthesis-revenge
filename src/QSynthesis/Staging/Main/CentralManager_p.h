#ifndef CENTRALMANAGERPRIVATE_H
#define CENTRALMANAGERPRIVATE_H

#include "BaseManager_p.h"

#include "CentralManager.h"

class CentralManagerPrivate : public BaseManagerPrivate {
    Q_DECLARE_PUBLIC(CentralManager)
public:
    CentralManagerPrivate();
    ~CentralManagerPrivate();

    void init();

    MainWindow *w;
};

#endif // CENTRALMANAGERPRIVATE_H
