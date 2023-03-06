#include "NativeWindowPlugin.h"
#include "NativeHandle.h"

NativeWindowPlugin::NativeWindowPlugin(QObject *parent) : IWindowHandlePlugin(parent) {
}

NativeWindowPlugin::~NativeWindowPlugin() {
}

IWindowHandle *NativeWindowPlugin::create(const QString &key, QMainWindow *win) {
    Q_UNUSED(key);
    return new NativeHandle(win);
}
