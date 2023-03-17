#include "ICore.h"
#include "ICore_p.h"

#include "Action/ActionSystem.h"
#include "Window/WindowSystem.h"

#include <QApplication>

namespace Core {

    ICorePrivate::ICorePrivate() : q_ptr(nullptr) {
        actionSystem = nullptr;
        windowSystem = nullptr;
    }

    void ICorePrivate::init() {
        qApp->setQuitOnLastWindowClosed(false);

        actionSystem = new ActionSystem(q_ptr);
        windowSystem = new WindowSystem(q_ptr);
    }

    static ICore *m_instance = nullptr;

    ICore *ICore::instance() {
        return m_instance;
    }

    ActionSystem *ICore::actionSystem() const {
        return d_ptr->actionSystem;
    }

    WindowSystem *ICore::windowSystem() const {
        return d_ptr->windowSystem;
    }

    ICore::ICore(QObject *parent) : ICore(*new ICorePrivate(), parent) {
    }

    ICore::~ICore() {
        m_instance = nullptr;
    }

    ICore::ICore(ICorePrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        m_instance = this;

        d.q_ptr = this;
        d.init();
    }
}