#include "QsPluginManager.h"
#include "private/QsPluginManager_p.h"

#include "Api/ICompressEngine.h"
#include "QsSystem.h"

#include "QsMath.h"

QSAPI_USING_NAMESPACE

Q_SINGLETON_DECLARE(QsPluginManager)

QsPluginManager::QsPluginManager(QObject *parent)
    : QsPluginManager(*new QsPluginManagerPrivate(), parent) {
}

QsPluginManager::~QsPluginManager() {
}

bool QsPluginManager::load() {
    return true;
}

bool QsPluginManager::save() {
    return true;
}

QsPluginDir *QsPluginManager::addPluginSet(const QString &key, const PluginSetOptions &opt) {
    Q_D(QsPluginManager);
    auto pd = QSharedPointer<QsPluginDir>::create();
    pd->load(opt.dir);

    QsPluginManagerPrivate::PluginSetInfo info;
    info.dir = opt.dir;
    info.categoryNameFunc = opt.categoryNameFunc;
    info.d_ptr = pd;

    d->pluginSets.insert(key, info);
    return pd.data();
}

void QsPluginManager::removePluginSet(const QString &key) {
    Q_D(QsPluginManager);
    d->pluginSets.remove(key);
}

QsPluginDir *QsPluginManager::pluginSet(const QString &key) {
    Q_D(QsPluginManager);
    auto it = d->pluginSets.find(key);
    if (it != d->pluginSets.end()) {
        return it.value().d_ptr.data();
    }
    return nullptr;
}

QPluginLoader *QsPluginManager::loadInternalPlugin(QsCoreConfig::InternalPlugins id) {
    QPluginLoader *loader = nullptr;

    switch (id) {
        case QsCoreConfig::AudioDecoder:
        case QsCoreConfig::AudioEncoder:
        case QsCoreConfig::AudioPlayback:
            break;
        case QsCoreConfig::CompressEngine: {
            QString name = qAppConf->internalPlugin(id);
            loader = new QPluginLoader(QsOs::toLibFile("compressengines", name));
            if (!INamePlugin::load<ICompressEngine>(loader)) {
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
