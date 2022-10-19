#include "PlainWindow_p.h"

#include "Api/IWindowFactory.h"

PlainWindowPrivate::PlainWindowPrivate() {
}

PlainWindowPrivate::~PlainWindowPrivate() {
    if (winHandle) {
        delete winHandle;
    }
    if (loader) {
        loader->unload();
        delete loader;
        qDebug() << "NativeWindow plugin unloaded.";
    }
}

void PlainWindowPrivate::init() {
    Q_Q(PlainWindow);

    loader = nullptr;
    winHandle = nullptr;

    auto loader = new QPluginLoader("windowfactories/NativeWindow");
    auto instance = loader->instance();
    if (instance) {
        auto avc = qobject_cast<IWindowFactory *>(instance);
        if (!avc) {
            qDebug() << "NativeWindow plugin not compatible.";
            delete instance;
            loader->unload();
            delete loader;
        } else {
            qDebug() << "NativeWindow plugin loaded.";
            qDebug() << loader->metaData();
            winHandle = avc->create(q);
            winHandle->setup();
            this->loader = loader;
        }
    } else {
        qDebug() << "Failed to load plugin:" << loader->errorString();

        delete loader;
    }
}
