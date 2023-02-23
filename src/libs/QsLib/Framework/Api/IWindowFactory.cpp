#include "IWindowFactory.h"

QSAPI_USING_NAMESPACE

IWindowFactory::IWindowFactory(QObject *parent) : QMNamePlugin(parent) {
}

IWindowFactory::~IWindowFactory() {
}

IWindowHandle *IWindowFactory::create(QMainWindow *win) {
    return new IWindowHandle(win);
}
