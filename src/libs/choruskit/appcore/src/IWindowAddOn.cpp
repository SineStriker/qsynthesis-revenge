#include "IWindowAddOn.h"
#include "IWindowAddOn_p.h"

namespace Core {

    IWindowAddOnPrivate::IWindowAddOnPrivate() : q_ptr(nullptr) {
    }

    IWindowAddOnPrivate::~IWindowAddOnPrivate() {
    }

    void IWindowAddOnPrivate::init() {
    }

    IWindowAddOnFactory::~IWindowAddOnFactory() {
    }

    bool IWindowAddOnFactory::predicate(IWindow *handle) const {
        return true;
    }

    IWindowAddOn::IWindowAddOn(QObject *parent) : IWindowAddOn(*new IWindowAddOnPrivate(), parent) {
    }

    IWindowAddOn::~IWindowAddOn() {
    }

    IWindow *IWindowAddOn::windowHandle() const {
        return d_ptr->iWin;
    }

    IWindowAddOn::IWindowAddOn(IWindowAddOnPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.init();
    }
}