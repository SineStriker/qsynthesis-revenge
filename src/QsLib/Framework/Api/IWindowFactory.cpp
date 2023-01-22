#include "IWindowFactory.h"

IWindowFactory::IWindowFactory(QObject *parent) : QObject(parent) {
}

IWindowFactory::~IWindowFactory() {
}

IWindowHandle *IWindowFactory::create(QMainWindow *win) {
    return new IWindowHandle(win);
}
