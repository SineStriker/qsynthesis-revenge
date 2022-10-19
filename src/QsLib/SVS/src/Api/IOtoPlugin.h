#ifndef IOTOPLUGIN_H
#define IOTOPLUGIN_H

#include "ISVSPlugin.h"

#include "Utau/QOtoItem.h"

#include "qssvs_global.h"

class QSSVS_API IOtoPlugin : public ISVSPlugin {
    Q_OBJECT
public:
    IOtoPlugin(QObject *parent = nullptr);
    ~IOtoPlugin();

    struct PluginArguments {
        QString rootPath;
        QString currentPath;
        QList<QOtoItem> selectedItems;
    };

    /**
     * @brief exec: Start plugin and block main thread
     * @param args: Voice bank paths and selections
     * @param userdata: User data if necessary
     * @return 0 as committed, 1 as aborted and negative number as error
     */
    virtual int exec(const PluginArguments &args, void *userdata) = 0;
};

#define IOtoPlugin_IID "QSynthesis.Plugin.SVS.OtoPlugin"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IOtoPlugin, IOtoPlugin_IID)
QT_END_NAMESPACE

#endif // IOTOPLUGIN_H
