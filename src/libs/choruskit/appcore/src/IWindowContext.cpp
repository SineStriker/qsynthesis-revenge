#include "IWindowContext.h"
#include "IWindowContext_p.h"

namespace Core {

    IWindowContextPrivate::IWindowContextPrivate() {
    }

    IWindowContextPrivate::~IWindowContextPrivate() {
    }

    void IWindowContextPrivate::init() {
        dt = QDateTime::currentDateTime();
    }

    IWindowContext::IWindowContext(Core::IWindow *winHandle, QObject *parent)
        : IWindowContext(*new IWindowContextPrivate(), winHandle, parent) {
    }

    IWindowContext::~IWindowContext() {
    }

    Core::IWindow *IWindowContext::windowHandle() const {
        Q_D(const IWindowContext);
        return d->iWin;
    }

    QDateTime IWindowContext::dateTime() const {
        Q_D(const IWindowContext);
        return d->dt;
    }

    IWindowContext::IWindowContext(IWindowContextPrivate &d, Core::IWindow *winHandle, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.iWin = winHandle;

        d.init();
    }

}
