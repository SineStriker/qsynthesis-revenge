#include "ActionSystem.h"
#include "ActionSystem_p.h"

namespace Core {

        ActionSystemPrivate::ActionSystemPrivate() : q_ptr(nullptr) {
        }

        void ActionSystemPrivate::init() {
        }

    ActionSystem *m_instance = nullptr;

    ActionInsertRuleSet ActionSystem::rule(const QString &id) {
        return d_ptr->rules[id];
    }

    void ActionSystem::buildMenuBar(const QString &context, QList<ActionItem *> &actionItems, QMenuBar *menuBar) {
        menuBar->clear();
    }

    void ActionSystem::buildMenu(const QString &context, QList<ActionItem *> &actionItems, QMenu *menu) {
        menu->clear();
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