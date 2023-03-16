#include "WindowSystem.h"
#include "WindowSystem_p.h"

#include "IWindow_p.h"

#include <QApplication>
#include <QDebug>

namespace Core {

    WindowSystemPrivate::WindowSystemPrivate() : q_ptr(nullptr) {
    }

    void WindowSystemPrivate::init() {
    }

    void WindowSystemPrivate::_q_iWindowDestroyed() {
        auto iWin = qobject_cast<IWindow *>(sender());
        emit q_ptr->windowDestroyed(iWin);
        iWindows.remove(iWin);
    }

    static WindowSystem *m_instance = nullptr;

    void WindowSystem::addWindow(IWindowFactory *factory) {
        if (!factory) {
            qWarning() << "Core::WindowSystem::addWindow(): trying to add null creator";
            return;
        }
        if (d_ptr->windowFactories.contains(factory->id())) {
            qWarning() << "Core::WindowSystem::addWindow(): trying to add duplicated creator";
            return;
        }
        d_ptr->windowFactories.insert(factory->id(), factory);
    }

    void WindowSystem::removeWindow(IWindowFactory *factory) {
        if (factory == nullptr) {
            qWarning() << "Core::WindowSystem::removeWindow(): trying to remove null creator";
            return;
        }
        removeWindow(factory->id());
    }

    void WindowSystem::removeWindow(const QString &id) {
        auto it = d_ptr->windowFactories.find(id);
        if (it == d_ptr->windowFactories.end()) {
            qWarning() << "Core::WindowSystem::removeWindow(): creator does not exist" << id;
            return;
        }
        d_ptr->windowFactories.erase(it);
    }

    void WindowSystem::addAddOn(IWindowAddOnFactory *factory) {
        if (!factory) {
            qWarning() << "Core::WindowSystem::addAddOn(): trying to add null add-on";
            return;
        }
        auto it = d_ptr->addOnFactories.insert(d_ptr->addOnFactories.end(), factory);
        d_ptr->addOnIndexes.insert(factory, it);
    }

    void WindowSystem::removeAddOn(IWindowAddOnFactory *factory) {
        if (!factory) {
            qWarning() << "Core::WindowSystem::removeAddOn(): trying to remove null add-on";
            return;
        }

        auto it = d_ptr->addOnIndexes.find(factory);
        if (it == d_ptr->addOnIndexes.end()) {
            qWarning() << "Core::WindowSystem::removeAddOn(): add-on does not exist" << factory;
            return;
        }
        d_ptr->addOnFactories.erase(it.value());
        d_ptr->addOnIndexes.erase(it);
    }

    IWindow *WindowSystem::createWindow(const QString &id, QWidget *parent) {
        auto it = d_ptr->windowFactories.find(id);
        if (it != d_ptr->windowFactories.end()) {
            qWarning() << "Core::WindowSystem::createWindow(): creator does not exist" << id;
            return nullptr;
        }

        auto factory = it.value();

        // Create window handle
        auto iWin = factory->creator() == IWindowFactory::Create ? factory->create() : factory->create(factory->id());
        if (!iWin) {
            qWarning() << "Core::WindowSystem::createWindow(): creator creates null instance" << id;
            return nullptr;
        }
        connect(iWin, &QObject::destroyed, d_ptr.data(), &WindowSystemPrivate::_q_iWindowDestroyed);

        // Create window
        auto window = iWin->createWindow(parent);
        window->setAttribute(Qt::WA_DeleteOnClose);

        iWin->d_ptr->window = window;
        connect(window, &QObject::destroyed, iWin->d_ptr.data(), &IWindowPrivate::_q_windowDestroyed);

        d_ptr->iWindows.insert(iWin);

        // Call all add-ons
        for (auto fac : qAsConst(d_ptr->addOnFactories)) {
            if (!fac->predicate(iWin)) {
                continue;
            }
            auto addOn = fac->create(iWin);
            iWin->d_ptr->addOns.push_back(addOn);
        }

        iWin->d_ptr->initAllAddOns();

        emit windowCreated(iWin);
        return iWin;
    }

    int WindowSystem::count() const {
        return d_ptr->iWindows.count();
    }

    QList<IWindow *> WindowSystem::windows() const {
        return d_ptr->iWindows.values();
    }

    WindowSystem::WindowSystem(QObject *parent) : WindowSystem(*new WindowSystemPrivate(), parent) {
    }

    WindowSystem::~WindowSystem() {
        m_instance = nullptr;
    }

    WindowSystem::WindowSystem(WindowSystemPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        m_instance = this;

        d.q_ptr = this;
        d.init();
    }
}