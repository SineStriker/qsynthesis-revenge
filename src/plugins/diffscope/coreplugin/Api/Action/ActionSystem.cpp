#include "ActionSystem.h"
#include "ActionSystem_p.h"

#include "ActionContext_p.h"

#include <QDebug>

namespace Core {

    ActionSystemPrivate::ActionSystemPrivate() : q_ptr(nullptr) {
    }

    void ActionSystemPrivate::init() {
    }

    static ActionSystem *m_instance = nullptr;

    void ActionSystem::addContext(ActionContext *context) {
        if (!context) {
            qWarning() << "Core::ActionSystem::addContext(): trying to add null context";
            return;
        }
        if (d_ptr->contexts.contains(context->id())) {
            qWarning() << "Core::ActionSystem::addContext(): trying to add duplicated context";
            return;
        }
        context->setParent(this);
        context->d_ptr->system = this;
        d_ptr->contexts.insert(context->id(), context);
    }

    void ActionSystem::removeContext(ActionContext *context) {
        if (context == nullptr) {
            qWarning() << "Core::ActionSystem::removeContext(): trying to remove null context";
            return;
        }
        removeContext(context->id());
    }

    void ActionSystem::removeContext(const QString &id) {
        auto it = d_ptr->contexts.find(id);
        if (it == d_ptr->contexts.end()) {
            qWarning() << "Core::ActionSystem::removeContext(): context does not exist" << id;
            return;
        }

        auto context = it.value();
        context->setParent(nullptr);
        context->d_ptr->system = nullptr;
        d_ptr->contexts.erase(it);
    }

    ActionContext *ActionSystem::context(const QString &id) const {
        auto it = d_ptr->contexts.find(id);
        if (it != d_ptr->contexts.end()) {
            return it.value();
        }
        return nullptr;
    }

    QList<ActionContext *> ActionSystem::contexts() const {
        return d_ptr->contexts.values();
    }

    QStringList ActionSystem::contextIds() const {
        return d_ptr->contexts.keys();
    }

    ActionSystem::ActionSystem(QObject *parent) : ActionSystem(*new ActionSystemPrivate(), parent) {
    }

    ActionSystem::~ActionSystem() {
        m_instance = nullptr;
    }

    ActionSystem::ActionSystem(ActionSystemPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        m_instance = this;

        d.q_ptr = this;
        d.init();
    }

}