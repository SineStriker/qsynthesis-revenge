#include "ActionSystem.h"
#include "ActionSystem_p.h"

#include "ActionContext_p.h"

#include <QDebug>

namespace Core {

    ActionSystemPrivate::ActionSystemPrivate() : q_ptr(nullptr) {
    }

    ActionSystemPrivate::~ActionSystemPrivate() {
    }

    void ActionSystemPrivate::init() {
    }

    static ActionSystem *m_instance = nullptr;

    ActionSystem::ActionSystem(QObject *parent) : ActionSystem(*new ActionSystemPrivate(), parent) {
    }

    ActionSystem::~ActionSystem() {
        m_instance = nullptr;
    }

    void ActionSystem::addContext(ActionContext *context) {
        Q_D(ActionSystem);
        if (!context) {
            qWarning() << "Core::ActionSystem::addContext(): trying to add null context";
            return;
        }
        if (d->contexts.contains(context->id())) {
            qWarning() << "Core::ActionSystem::addContext(): trying to add duplicated context:" << context->id();
            return;
        }
        context->setParent(this);
        d->contexts.insert(context->id(), context);
    }

    void ActionSystem::removeContext(ActionContext *context) {
        if (context == nullptr) {
            qWarning() << "Core::ActionSystem::removeContext(): trying to remove null context";
            return;
        }
        removeContext(context->id());
    }

    void ActionSystem::removeContext(const QString &id) {
        Q_D(ActionSystem);
        auto it = d->contexts.find(id);
        if (it == d->contexts.end()) {
            qWarning() << "Core::ActionSystem::removeContext(): context does not exist:" << id;
            return;
        }

        auto context = it.value();
        context->setParent(nullptr);
        d->contexts.erase(it);
    }

    ActionContext *ActionSystem::context(const QString &id) const {
        Q_D(const ActionSystem);
        auto it = d->contexts.find(id);
        if (it != d->contexts.end()) {
            return it.value();
        }
        return nullptr;
    }

    QList<ActionContext *> ActionSystem::contexts() const {
        Q_D(const ActionSystem);
        return d->contexts.values();
    }

    QStringList ActionSystem::contextIds() const {
        Q_D(const ActionSystem);
        return d->contexts.keys();
    }

    QMap<QString, QStringList> ActionSystem::stateCache(const QString &id) {
        Q_D(const ActionSystem);
        return d->stateCaches.value(id, {});
    }

    void ActionSystem::setStateCache(const QString &id, const QMap<QString, QStringList> &state) {
        Q_D(ActionSystem);
        d->stateCaches.insert(id, state);
    }

    ActionSystem::ActionSystem(ActionSystemPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        m_instance = this;

        d.q_ptr = this;
        d.init();
    }

}