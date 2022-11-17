#include "NativeWindowFactory.h"
#include "NativeHandle.h"

#include "Kernel/LocalLinguist.h"
#include "Kernel/MultistyleHandle.h"

#include <QTranslator>

class NativeWindowFactoryPrivate {
    Q_DECLARE_PUBLIC(NativeWindowFactory)
public:
    void init();

    NativeWindowFactory *q_ptr;

    LocalLinguist *ll;
};

void NativeWindowFactoryPrivate::init() {
    Q_Q(NativeWindowFactory);

    ll = new LocalLinguist(q);
}


// ----------------------------------------------------------------------------


NativeWindowFactory::NativeWindowFactory(QObject *parent)
    : IWindowFactory(parent), d_ptr(new NativeWindowFactoryPrivate()) {
    d_ptr->q_ptr = this;
    d_ptr->init();

    Q_TR_NOTIFY(NativeWindowFactory);
    Q_SS_NOTIFY(NativeWindowFactory);
}

NativeWindowFactory::~NativeWindowFactory() {
}

void NativeWindowFactory::reloadStrings(int locale) {
    Q_D(NativeWindowFactory);
    switch (locale) {
        case MultimodeHandle::UnitedStates:
            d->ll->eliminate();
            break;
        case MultimodeHandle::China:
            d->ll->translateOnly(":/translations/NativeWindow_zh_CN.qm");
            break;
        case MultimodeHandle::HongKong:
            d->ll->translateOnly(":/translations/NativeWindow_zh_HK.qm");
            break;
        case MultimodeHandle::Japan:
            d->ll->translateOnly(":/translations/NativeWindow_ja_JP.qm");
            break;
        default:
            break;
    }
}

void NativeWindowFactory::reloadScreen(int theme) {
    Q_UNUSED(theme);
}

IWindowHandle *NativeWindowFactory::create(QMainWindow *win) {
    return new NativeHandle(win);
}
