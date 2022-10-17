#ifndef EXTENTMANAGER_H
#define EXTENTMANAGER_H

/*
 * Usage: Manage stylesheets/extensions/devices
 *
 *
 */

#include "Vendor/Objects/BasicManager.h"
#include "qsutils_macros.h"

#define qExt ExtentManager::instance()

class ExtentManagerPrivate;

class ExtentManager : public BasicManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(ExtentManager)

    Q_SINGLETON(ExtentManager)
public:
    ExtentManager(QObject *parent = nullptr);
    ~ExtentManager();

public:
    bool load();
    bool save();

    void reloadScreen();

protected:
    ExtentManager(ExtentManagerPrivate &d, QObject *parent = nullptr);
};

#endif // EXTENTMANAGER_H
