#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>

#include "QMCCGlobal.h"

QMCC_BEGIN_NAMESPACE

class PluginManagerPrivate;

class PluginManager : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(PluginManager)
public:
    PluginManager(QObject *parent = nullptr);
    ~PluginManager();

protected:
    PluginManager(PluginManagerPrivate &d, QObject *parent = nullptr);

    QScopedPointer<PluginManagerPrivate> d_ptr;
};

QMCC_END_NAMESPACE

#endif // PLUGINMANAGER_H
