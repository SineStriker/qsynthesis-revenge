#ifndef IPLUGIN_H
#define IPLUGIN_H

#include "QMCCGlobal.h"
#include "QMNamePlugin.h"

QMCC_BEGIN_NAMESPACE

class IPluginPrivate;

class IPlugin : public QMNamePlugin {
    Q_OBJECT
    Q_DECLARE_PRIVATE(IPlugin)
public:
    IPlugin(QObject *parent = nullptr);
    ~IPlugin();

protected:
    IPlugin(IPluginPrivate &d, QObject *parent = nullptr);

    QScopedPointer<IPluginPrivate> d_ptr;
};

QMCC_END_NAMESPACE

#endif // IPLUGIN_H