#ifndef EXTENSIONMANAGERPRIVATE_H
#define EXTENSIONMANAGERPRIVATE_H

#include "BaseManager_p.h"

#include "ExtensionManager.h"

class ExtensionManagerPrivate : public BaseManagerPrivate {
    Q_DECLARE_PUBLIC(ExtensionManager)
public:
    ExtensionManagerPrivate();
    ~ExtensionManagerPrivate();

    void init();

    static void reloadAppFont();
};

#endif // EXTENSIONMANAGERPRIVATE_H
