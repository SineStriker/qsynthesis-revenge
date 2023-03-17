#include "IWindowAddOn.h"
#include "IWindowAddOn_p.h"

namespace Core {

    IWindowAddOnPrivate::IWindowAddOnPrivate() : q_ptr(nullptr) {
    }

    void IWindowAddOnPrivate::init() {
    }

    IWindowAddOnFactory::~IWindowAddOnFactory() {
    }

    bool IWindowAddOnFactory::predicate(IWindow *iWindow) const {
        return true;
    }

    IWindowAddOn::IWindowAddOn(QObject *parent) : IWindowAddOn(*new IWindowAddOnPrivate(), parent) {
    }

    IWindowAddOn::~IWindowAddOn() {
    }

    IWindow *IWindowAddOn::iWin() const {
        return d_ptr->iWin;
    }

    IWindowAddOn::IWindowAddOn(IWindowAddOnPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.init();
    }
}