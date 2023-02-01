#include "PlainWindow_p.h"

#include "Api/IWindowFactory.h"
#include "Managers/QsPluginManager.h"

#include "QsSystem.h"

PlainWindowPrivate::PlainWindowPrivate() {
}

PlainWindowPrivate::~PlainWindowPrivate() {
    if (winHandle) {
        delete winHandle;
        winHandle = nullptr;
    }
    if (loader) {
        loader->unload();
        delete loader;
    }
}

void PlainWindowPrivate::init() {
    Q_Q(PlainWindow);
    winHandle = nullptr;
    // loader = nullptr;
    loader = QsPluginManager::loadInternalPlugin(QsCoreConfig::WindowFactory);
    if (loader) {
        auto fac = qobject_cast<IWindowFactory *>(loader->instance());
        Q_ASSERT(fac);
        fac->setupPath(QsSys::PathFindDirPath(loader->fileName()));

        winHandle = fac->create(q);
        winHandle->setup();
    }
}
