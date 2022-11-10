#include "NativeWindowFactory.h"

#include "Widgets/NativeHandle.h"

NativeWindowFactory::NativeWindowFactory(QObject *parent) : IWindowFactory(parent) {
}

NativeWindowFactory::~NativeWindowFactory() {
}

IWindowHandle *NativeWindowFactory::create(QMainWindow *win) {
    return new NativeHandle(win);
}
