#include "NativeWindowFactory.h"
#include "NativeHandle.h"

NativeWindowFactory::NativeWindowFactory(QObject *parent) : IWindowFactory(parent) {
}

NativeWindowFactory::~NativeWindowFactory() {
}

IWindowHandle *NativeWindowFactory::create(QMainWindow *win) {
    return new NativeHandle(win);
}
