#include "NativeWindowFactory.h"
#include "NativeHandle.h"

#include "CDecorateDir.h"
#include "QsCoreConfig.h"

#include "CDecorator.h"
#include "QsSystem.h"

#include <QTranslator>

class NativeWindowFactoryPrivate {
public:
    void init();

    CDecorateDir dd;
    NativeWindowFactory *q_ptr;
};

void NativeWindowFactoryPrivate::init() {
    qIDec->addThemeTemplate("NativeWindow", ":/themes/window-bar.qss.in");

    dd.setDir(qAppConf->appDir(QsCoreConfig::AppShare));
    dd.loadDefault("NativeWindow");
}

// ----------------------------------------------------------------------------

NativeWindowFactory::NativeWindowFactory(QObject *parent)
    : IWindowFactory(parent), d_ptr(new NativeWindowFactoryPrivate()) {
    d_ptr->q_ptr = this;
    d_ptr->init();
}

NativeWindowFactory::~NativeWindowFactory() {
}

IWindowHandle *NativeWindowFactory::create(QMainWindow *win) {
    return new NativeHandle(win);
}
