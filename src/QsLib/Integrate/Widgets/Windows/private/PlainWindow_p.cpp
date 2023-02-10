#include "PlainWindow_p.h"

#include "Api/IWindowFactory.h"
#include "Managers/QsPluginManager.h"

#include "QsStartInfo.h"
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
#ifndef Q_OS_MAC
    winHandle = nullptr;
    loader = QsPluginManager::instance()->loadInternalPlugin(QsCoreConfig::WindowFactory);
    if (loader) {
        winHandle = qobject_cast<IWindowFactory *>(loader->instance())->create(q);
        winHandle->setup();
    }
#endif
    q->setWindowTitle(qIStup->windowTitle());
}
