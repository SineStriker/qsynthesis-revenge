#ifndef IOTOPLUGIN_H
#define IOTOPLUGIN_H

#include <QObject>

#include "Utau/QOtoItem.h"

#include "qssvs_global.h"

class QSSVS_API IOtoPlugin : public QObject {
    Q_OBJECT
public:
    IOtoPlugin(QObject *parent = nullptr);
    ~IOtoPlugin();

    struct PluginArguments {
        QString rootPath;
        QString currentPath;
        QList<QOtoItem> selectedItems;
    };

    virtual int exec(const PluginArguments &args, void *userdata) = 0;
};

#define IOtoPlugin_IID "QSynthesis.Plugin.SVS.OtoPlugin"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IOtoPlugin, IOtoPlugin_IID)
QT_END_NAMESPACE

#endif // IOTOPLUGIN_H
