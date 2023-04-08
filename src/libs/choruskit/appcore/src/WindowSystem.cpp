#include "WindowSystem.h"
#include "WindowSystem_p.h"

#include "IWindowAddOn_p.h"
#include "IWindow_p.h"
#include "WindowCloseFilter_p.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDebug>

namespace Core {

    WindowSystemPrivate::WindowSystemPrivate() : q_ptr(nullptr) {
    }

    WindowSystemPrivate::~WindowSystemPrivate() {
    }

    void WindowSystemPrivate::init() {
    }

    void WindowSystemPrivate::_q_iWindowClosed() {
        Q_Q(WindowSystem);

        auto iWin = qobject_cast<IWindow *>(sender());
        emit q->windowDestroyed(iWin);
        iWindows.remove(iWin);
        windowMap.remove(iWin->window());

        if (iWindows.isEmpty()) {
            QCloseEvent e;
            qApp->sendEvent(q, &e);
            if (e.isAccepted()) {
                // auto e2 = new QEvent(QEvent::Quit);
                // qApp->postEvent(qApp, e2);
                qApp->quit();
            }
        }
    }

    static WindowSystem *m_instance = nullptr;

    WindowSystem::WindowSystem(QObject *parent) : WindowSystem(*new WindowSystemPrivate(), parent) {
    }

    WindowSystem::~WindowSystem() {
        Q_D(WindowSystem);

        m_instance = nullptr;

        // Remove all managed factories
        for (auto &item : qAsConst(d->windowFactories)) {
            delete item;
        }

        for (auto &item : qAsConst(d->addOnFactories)) {
            delete item;
        }
    }

    bool WindowSystem::addWindow(IWindowFactory *factory) {
        Q_D(WindowSystem);
        if (!factory) {
            qWarning() << "Core::WindowSystem::addWindow(): trying to add null factory";
            return false;
        }
        if (d->windowFactories.contains(factory->id())) {
            qWarning() << "Core::WindowSystem::addWindow(): trying to add duplicated factory:" << factory->id();
            return false;
        }
        d->windowFactories.append(factory->id(), factory);
        return true;
    }

    bool WindowSystem::removeWindow(IWindowFactory *factory) {
        if (factory == nullptr) {
            qWarning() << "Core::WindowSystem::removeWindow(): trying to remove null factory";
            return false;
        }
        return removeWindow(factory->id());
    }

    bool WindowSystem::removeWindow(const QString &id) {
        Q_D(WindowSystem);
        auto it = d->windowFactories.find(id);
        if (it == d->windowFactories.end()) {
            qWarning() << "Core::WindowSystem::removeWindow(): creator does not exist:" << id;
            return false;
        }
        d->windowFactories.erase(it);
        return true;
    }

    QList<IWindowFactory *> WindowSystem::windowFactories() const {
        Q_D(const WindowSystem);
        return d->windowFactories.values();
    }

    void WindowSystem::removeWindowFactories() {
        Q_D(WindowSystem);
        d->windowFactories.clear();
    }

    bool WindowSystem::addAddOn(IWindowAddOnFactory *factory) {
        Q_D(WindowSystem);
        if (!factory) {
            qWarning() << "Core::WindowSystem::addAddOn(): trying to add null factory";
            return false;
        }
        d->addOnFactories.append(factory);
        return true;
    }

    bool WindowSystem::removeAddOn(IWindowAddOnFactory *factory) {
        Q_D(WindowSystem);
        if (!factory) {
            qWarning() << "Core::WindowSystem::removeAddOn(): trying to remove null factory";
            return false;
        }

        if (!d->addOnFactories.remove(factory)) {
            qWarning() << "Core::WindowSystem::removeAddOn(): factory does not exist:" << factory;
            return false;
        }
        return true;
    }

    QList<IWindowAddOnFactory *> WindowSystem::addOnFactories() const {
        Q_D(const WindowSystem);
        return d->addOnFactories.values();
    }

    void WindowSystem::removeAddOnFactories() {
        Q_D(WindowSystem);
        d->addOnFactories.clear();
    }

    IWindow *WindowSystem::createWindow(const QString &id, QWidget *parent) {
        Q_D(WindowSystem);

        auto it = d->windowFactories.find(id);
        if (it == d->windowFactories.end()) {
            qWarning() << "Core::WindowSystem::createWindow(): window factory does not exist:" << id;
            return nullptr;
        }

        auto factory = it.value();

        // Create window handle
        auto iWin = factory->creator() == IWindowFactory::Create ? factory->create(nullptr)
                                                                 : factory->create(factory->id(), nullptr);
        if (!iWin) {
            qWarning() << "Core::WindowSystem::createWindow(): window factory creates null instance:" << id;
            return nullptr;
        }

        d->iWindows.append(iWin);
        connect(iWin, &IWindow::closed, d, &WindowSystemPrivate::_q_iWindowClosed);

        // Create window
        auto window = iWin->createWindow(parent);

        // Add to indexes
        d->windowMap.insert(window, iWin);

        window->setAttribute(Qt::WA_DeleteOnClose);
        iWin->d_ptr->setWindow(window, d);

        emit windowCreated(iWin);

        window->show();

        return iWin;
    }

    IWindow *WindowSystem::findWindow(QWidget *window) const {
        Q_D(const WindowSystem);
        return d->windowMap.value(window, nullptr);
    }

    int WindowSystem::count() const {
        Q_D(const WindowSystem);
        return d->iWindows.size();
    }

    QList<IWindow *> WindowSystem::windows() const {
        Q_D(const WindowSystem);
        return d->iWindows.values();
    }

    IWindow *WindowSystem::firstWindow() const {
        Q_D(const WindowSystem);
        return d->iWindows.isEmpty() ? nullptr : *d->iWindows.begin();
    }

    WindowSystem::WindowSystem(WindowSystemPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        m_instance = this;

        d.q_ptr = this;
        d.init();
    }
}