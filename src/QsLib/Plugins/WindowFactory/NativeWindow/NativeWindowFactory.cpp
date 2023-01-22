#include "NativeWindowFactory.h"
#include "NativeHandle.h"

#include "CDecorator.h"
#include "Tools/LocalLinguist.h"

#include <QTranslator>

class NativeWindowFactoryPrivate {
public:
    void init();

    NativeWindowFactory *q_ptr;

    LocalLinguist *ll;
};

void NativeWindowFactoryPrivate::init() {
    ll = new LocalLinguist(q_ptr);
    ll->addLocale(QsDecorator::UnitedStates, {});
    ll->addLocale(QsDecorator::China, {":/translations/NativeWindow_zh_CN.qm"});
    ll->addLocale(QsDecorator::HongKong, {":/translations/NativeWindow_zh_HK.qm"});
    ll->addLocale(QsDecorator::Japan, {":/translations/NativeWindow_ja_JP.qm"});
    ll->reloadStrings(qIDec->locale());
    qIDec->addThemes({":/themes/window-bar.qss.in"});
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
