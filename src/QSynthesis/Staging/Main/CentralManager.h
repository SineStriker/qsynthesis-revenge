#ifndef CENTRALMANAGER_H
#define CENTRALMANAGER_H

#include "BaseManager.h"

class MainWindow;
class CentralManagerPrivate;

class CentralManager : public BaseManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CentralManager)
public:
    CentralManager(MainWindow *parent = nullptr);
    ~CentralManager();

protected:
    CentralManager(CentralManagerPrivate &d, MainWindow *parent = nullptr);
};

#endif // CENTRALMANAGER_H
