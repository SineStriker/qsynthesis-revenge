#include "IWindowFactory.h"

QSAPI_USING_NAMESPACE

IWindowFactory::IWindowFactory(QObject *parent) : INamePlugin(parent) {
}

IWindowFactory::~IWindowFactory() {
}

IWindowHandle *IWindowFactory::create(QMainWindow *win) {
    return new IWindowHandle(win);
}
