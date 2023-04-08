#ifndef NATIVEWINDOWPLUGIN_H
#define NATIVEWINDOWPLUGIN_H

#include "Api/interfaces/IWindowHandlePlugin.h"

QSAPI_USING_NAMESPACE

class Q_DECL_EXPORT NativeWindowPlugin : public IWindowHandlePlugin {
    Q_OBJECT
    Q_INTERFACES(QsApi::IWindowHandlePlugin)
    Q_PLUGIN_METADATA(IID QsApi_IWindowHandlePlugin_IID FILE "plugin.json")
public:
    explicit NativeWindowPlugin(QObject *parent = nullptr);
    ~NativeWindowPlugin();

public:
    IWindowHandle *create(const QString &key, QMainWindow *win) override;
};

#endif // NATIVEWINDOWPLUGIN_H
