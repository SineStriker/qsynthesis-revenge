#ifndef QSPLUGINMANAGER_H
#define QSPLUGINMANAGER_H

#include <QPluginLoader>

#include "QsCoreConfig.h"
#include "QsAbstractManager.h"

#define qsPluginMgr QsPluginManager::instance()

class QsPluginManagerPrivate;

class QSFRAMEWORK_API QsPluginManager : public QsAbstractManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsPluginManager)
    Q_SINGLETON(QsPluginManager)
public:
    explicit QsPluginManager(QObject *parent = nullptr);
    ~QsPluginManager();

public:
    virtual QPluginLoader *loadInternalPlugin(QsCoreConfig::InternalPlugins id);

protected:
    QsPluginManager(QsPluginManagerPrivate &d, QObject *parent = nullptr);
};

#endif // QSPLUGINMANAGER_H
