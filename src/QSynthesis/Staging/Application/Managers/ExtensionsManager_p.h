#ifndef EXTENSIONSMANAGERPRIVATE_H
#define EXTENSIONSMANAGERPRIVATE_H

#include "BaseManager_p.h"

#include "ExtensionsManager.h"

class ExtensionsManagerPrivate : public BaseManagerPrivate {
    Q_DECLARE_PUBLIC(ExtensionsManager)
public:
    ExtensionsManagerPrivate();
    ~ExtensionsManagerPrivate();

    void init();

    static void reloadAppFont();
};

#endif // EXTENSIONSMANAGERPRIVATE_H
