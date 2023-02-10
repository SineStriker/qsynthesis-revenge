#ifndef QSPLUGINMANAGER_H
#define QSPLUGINMANAGER_H

#include <QPluginLoader>

#include "QsAbstractManager.h"
#include "QsCoreConfig.h"
#include "QsPluginDir.h"

#include "QsMacros.h"

#define qsPluginMgr QsPluginManager::instance()

class QsPluginManagerPrivate;

class QSBASE_API QsPluginManager : public QsAbstractManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsPluginManager)
    Q_SINGLETON(QsPluginManager)
public:
    QsPluginManager(QObject *parent = nullptr);
    ~QsPluginManager();

public:
    bool load() override;
    bool save() override;

    QsPluginDir *addPluginSet(const QString &key, const QString &dir);
    void removePluginSet(const QString &key);
    
    QsPluginDir *pluginSet(const QString &key);

    virtual QPluginLoader *loadInternalPlugin(QsCoreConfig::InternalPlugins id);

protected:
    QsPluginManager(QsPluginManagerPrivate &d, QObject *parent = nullptr);
};

#endif // QSPLUGINMANAGER_H
