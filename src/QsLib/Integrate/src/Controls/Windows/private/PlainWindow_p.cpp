#include "PlainWindow_p.h"

#include "Api/IWindowFactory.h"
#include "Managers/QsPluginManager.h"

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
    loader = QsPluginManager::loadInternalPlugin(QsDistConfig::WindowFactory);
    if (loader) {
        winHandle = qobject_cast<IWindowFactory *>(loader->instance())->create(q);
        winHandle->setup();
    }
}
