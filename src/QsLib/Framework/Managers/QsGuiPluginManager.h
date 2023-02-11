#ifndef QSGUIPLUGINMANAGER_H
#define QSGUIPLUGINMANAGER_H

#include "QsPluginManager.h"

class QsGuiPluginManagerPrivate;

class QsGuiPluginManager : public QsPluginManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsGuiPluginManager)
public:
    explicit QsGuiPluginManager(QObject *parent = nullptr);
    ~QsGuiPluginManager();

public:
    QPluginLoader *loadInternalPlugin(QsCoreConfig::InternalPlugins id) override;

protected:
    QsGuiPluginManager(QsGuiPluginManagerPrivate &d, QObject *parent = nullptr);
};

#endif // QSGUIPLUGINMANAGER_H
