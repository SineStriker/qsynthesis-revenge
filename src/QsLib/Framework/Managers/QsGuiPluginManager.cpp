#include "QsGuiPluginManager.h"
#include "private/QsGuiPluginManager_p.h"

#include "QMSystem.h"

#include "IWindowFactory.h"

QSAPI_USING_NAMESPACE

QsGuiPluginManager::QsGuiPluginManager(QObject *parent)
    : QsGuiPluginManager(*new QsGuiPluginManagerPrivate(), parent) {
}

QsGuiPluginManager::~QsGuiPluginManager() {
}

QPluginLoader *QsGuiPluginManager::loadInternalPlugin(QsCoreConfig::InternalPlugins id) {
    QPluginLoader *loader = nullptr;
    loader = QsPluginManager::loadInternalPlugin(id);
    if (loader) {
        return loader;
    }

    switch (id) {
        case QsCoreConfig::WindowFactory: {
            QString name = qAppConf->internalPlugin(id);
            loader = new QPluginLoader(QMOs::toLibFile("windowfactories", name));
            if (!QMNamePlugin::load<IWindowFactory>(loader)) {
                qDebug() << QString("QsPluginManager: Failed to load %1.").arg(name)
                         << loader->errorString();
                loader->unload();

                delete loader;
                loader = nullptr;
            }
            break;
        }
        default:
            break;
    }

    return loader;
}

QsGuiPluginManager::QsGuiPluginManager(QsGuiPluginManagerPrivate &d, QObject *parent)
    : QsPluginManager(d, parent) {
    d.init();
}
