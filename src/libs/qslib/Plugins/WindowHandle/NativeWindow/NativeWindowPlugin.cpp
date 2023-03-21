#include "NativeWindowPlugin.h"
#include "NativeHandle.h"

#include "QMDecorator.h"

NativeWindowPlugin::NativeWindowPlugin(QObject *parent) : IWindowHandlePlugin(parent) {
    qIDec->addThemeTemplate("NativeWindow", ":/themes/window-bar.qss.in");
}

NativeWindowPlugin::~NativeWindowPlugin() {
}

IWindowHandle *NativeWindowPlugin::create(const QString &key, QMainWindow *win) {
    Q_UNUSED(key);
    return new NativeHandle(win);
}
