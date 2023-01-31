#include "NativeWindowFactory.h"
#include "NativeHandle.h"

#include "CDecreateDir.h"

#include "CDecorator.h"

#include <QTranslator>

class NativeWindowFactoryPrivate {
public:
    void init();

    CDecreateDir dd;
    NativeWindowFactory *q_ptr;
};

void NativeWindowFactoryPrivate::init() {
    qIDec->addThemeTemplate("NativeWindow", ":/themes/window-bar.qss.in");
}

// ----------------------------------------------------------------------------

NativeWindowFactory::NativeWindowFactory(QObject *parent)
    : IWindowFactory(parent), d_ptr(new NativeWindowFactoryPrivate()) {
    d_ptr->q_ptr = this;
    d_ptr->init();
}

NativeWindowFactory::~NativeWindowFactory() {
}

void NativeWindowFactory::setupPath(const QString &path) {
    IWindowFactory::setupPath(path);

    d_ptr->dd.setDir(path);
    d_ptr->dd.loadDefault("NativeWindow");
}

IWindowHandle *NativeWindowFactory::create(QMainWindow *win) {
    return new NativeHandle(win);
}
