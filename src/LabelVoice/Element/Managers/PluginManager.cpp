#include "PluginManager.h"
#include "private/PluginManager_p.h"

#include "Api/ICompressEngine.h"
#include "Api/IWindowFactory.h"

#include "Helpers/MathHelper.h"

Q_SINGLETON_DECLARE(PluginManager)

#ifdef Q_OS_WINDOWS
#define ALL_FILES "*.*"
#else
#define ALL_FILES "*"
#endif

static QString toLibFile(const QString &dir, const QString &filename) {
#if defined(Q_OS_LINUX) || defined(__MINGW32__)
    return dir + "/lib" + filename;
#elif defined(Q_OS_WINDOWS)
    return dir + "/" + filename;
#else
    return dir + "/" + filename;
#endif
}

PluginManager::PluginManager(QObject *parent) : PluginManager(*new PluginManagerPrivate(), parent) {
}

PluginManager::~PluginManager() {
}

void PluginManager::load() {
    Q_D(PluginManager);

    // Avoid repeated calling
    d->unloadConverters();

    d->loadConverters();
}

void PluginManager::save() {
    Q_D(PluginManager);

    d->unloadConverters();
}

QString PluginManager::converterFilters() const {
    Q_D(const PluginManager);

    QStringList filters;
    for (auto info : qAsConst(d->converters)) {
        filters.append(QString("%1(%2)").arg(
            info.target,
            Math::batchReplace<QString>(info.filters, [](const QString &str) -> QString {
                return "*." + str;
            }).join(" ")));
    }
    filters.append(QString("%1(%2)").arg(tr("All Files"), ALL_FILES));

    return filters.join(";;");
}

ISVSConverter *PluginManager::searchConverter(const QString &suffix) const {
    Q_D(const PluginManager);
    auto it = d->converterMap.find(suffix.toLower());
    if (it == d->converterMap.end()) {
        return nullptr;
    }
    return qobject_cast<ISVSConverter *>(d->converters.at(it.value()).loader->instance());
}

QPluginLoader *PluginManager::loadInternalPlugin(LvDistConfig::InternalPlugins id) {
    QPluginLoader *loader = nullptr;

    switch (id) {
        case LvDistConfig::AudioDecoder:
        case LvDistConfig::AudioEncoder:
        case LvDistConfig::AudioPlayback:
            break;
        case LvDistConfig::CompressEngine: {
            QString name = qAppConf->internalPlugin(id);
            loader = new QPluginLoader(toLibFile("compressengines", name));
            if (!(loader->load() && qobject_cast<ICompressEngine *>(loader->instance()))) {
                qDebug() << QString("PluginManager: Failed to load %1.").arg(name)
                         << loader->errorString();
                loader->unload();
                delete loader;
                loader = nullptr;
            }
            break;
        }
        case LvDistConfig::WindowFactory: {
            QString name = qAppConf->internalPlugin(id);
            loader = new QPluginLoader(toLibFile("windowfactories", name));
            if (!(loader->load() && qobject_cast<IWindowFactory *>(loader->instance()))) {
                qDebug() << QString("PluginManager: Failed to load %1.").arg(name)
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

PluginManager::PluginManager(PluginManagerPrivate &d, QObject *parent) : BasicManager(d, parent) {
    construct();
    d.init();
}
