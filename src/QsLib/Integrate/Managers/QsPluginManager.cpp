#include "QsPluginManager.h"
#include "private/QsPluginManager_p.h"

#include "Api/ICompressEngine.h"
#include "Api/IWindowFactory.h"

#include "QsMath.h"

Q_SINGLETON_DECLARE(QsPluginManager)

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

QsPluginManager::QsPluginManager(QObject *parent)
    : QsPluginManager(*new QsPluginManagerPrivate(), parent) {
}

QsPluginManager::~QsPluginManager() {
}

bool QsPluginManager::load() {
    Q_D(QsPluginManager);

    // Avoid repeated calling
    d->unloadConverters();

    d->loadConverters();

    return true;
}

bool QsPluginManager::save() {
    Q_D(QsPluginManager);

    d->unloadConverters();

    return true;
}

QString QsPluginManager::converterFilters() const {
    Q_D(const QsPluginManager);

    QStringList filters;
    for (auto info : qAsConst(d->converters)) {
        filters.append(QString("%1(%2)").arg(
            info.target,
            QsMath::batchReplace<QString>(info.filters, [](const QString &str) -> QString {
                return "*." + str;
            }).join(" ")));
    }
    filters.append(QString("%1(%2)").arg(tr("All Files"), ALL_FILES));

    return filters.join(";;");
}

ISVSConverter *QsPluginManager::searchConverter(const QString &suffix) const {
    Q_D(const QsPluginManager);
    auto it = d->converterMap.find(suffix.toLower());
    if (it == d->converterMap.end()) {
        return nullptr;
    }
    return qobject_cast<ISVSConverter *>(d->converters.at(it.value()).loader->instance());
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
            loader = new QPluginLoader(toLibFile("compressengines", name));
            if (!(loader->load() && qobject_cast<ICompressEngine *>(loader->instance()))) {
                qDebug() << QString("QsPluginManager: Failed to load %1.").arg(name)
                         << loader->errorString();
                loader->unload();
                delete loader;
                loader = nullptr;
            }
            break;
        }
        case QsCoreConfig::WindowFactory: {
            QString name = qAppConf->internalPlugin(id);
            loader = new QPluginLoader(toLibFile("windowfactories", name));
            if (!(loader->load() && qobject_cast<IWindowFactory *>(loader->instance()))) {
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
