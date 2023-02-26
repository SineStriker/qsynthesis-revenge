#include "QsPluginManager.h"
#include "private/QsPluginManager_p.h"

#include "Api/IAudioDecoder.h"
#include "Api/IAudioEncoder.h"
#include "Api/IAudioPlayback.h"
#include "Api/ICompressEngine.h"
#include "Api/IWindowFactory.h"

#include "QMSystem.h"

QSAPI_USING_NAMESPACE

Q_SINGLETON_DECLARE(QsPluginManager)

QsPluginManager::QsPluginManager(QObject *parent)
    : QsPluginManager(*new QsPluginManagerPrivate(), parent) {
}

QsPluginManager::~QsPluginManager() {
}

QPluginLoader *QsPluginManager::loadInternalPlugin(QsCoreConfig::InternalPlugins id) {
    QPluginLoader *loader = nullptr;

    switch (id) {
        case QsCoreConfig::AudioDecoder: {
            QString name = qsConf->internalPlugin(id);
            loader = new QPluginLoader(QMOs::toLibFile("audiodecoders", name));
            if (!QMNamePlugin::load<IAudioDecoder>(loader)) {
                qDebug() << QString("QsPluginManager: Failed to load %1.").arg(name)
                         << loader->errorString();
                loader->unload();

                delete loader;
                loader = nullptr;
            }
            break;
        }
        case QsCoreConfig::AudioEncoder: {
            QString name = qsConf->internalPlugin(id);
            loader = new QPluginLoader(QMOs::toLibFile("audioencoders", name));
            if (!QMNamePlugin::load<IAudioEncoder>(loader)) {
                qDebug() << QString("QsPluginManager: Failed to load %1.").arg(name)
                         << loader->errorString();
                loader->unload();

                delete loader;
                loader = nullptr;
            }
            break;
        }
        case QsCoreConfig::AudioPlayback: {
            QString name = qsConf->internalPlugin(id);
            loader = new QPluginLoader(QMOs::toLibFile("audioplaybacks", name));
            if (!QMNamePlugin::load<IAudioPlayback>(loader)) {
                qDebug() << QString("QsPluginManager: Failed to load %1.").arg(name)
                         << loader->errorString();
                loader->unload();

                delete loader;
                loader = nullptr;
            }
            break;
        }
        case QsCoreConfig::CompressEngine: {
            QString name = qsConf->internalPlugin(id);
            loader = new QPluginLoader(QMOs::toLibFile("compressengines", name));
            if (!QMNamePlugin::load<ICompressEngine>(loader)) {
                qDebug() << QString("QsPluginManager: Failed to load %1.").arg(name)
                         << loader->errorString();
                loader->unload();

                delete loader;
                loader = nullptr;
            }
            break;
        }
        case QsCoreConfig::WindowFactory: {
            QString name = qsConf->internalPlugin(id);
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

QsPluginManager::QsPluginManager(QsPluginManagerPrivate &d, QObject *parent)
    : QsAbstractManager(d, parent) {
    construct();
    d.init();
}
