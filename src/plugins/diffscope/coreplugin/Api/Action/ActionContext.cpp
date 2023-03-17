#include "ActionContext.h"
#include "ActionContext_p.h"

namespace Core {

    ActionContextPrivate::ActionContextPrivate() : q_ptr(nullptr) {
    }

    void ActionContextPrivate::init() {
    }

    ActionContext::ActionContext(const QString &id, QObject *parent)
        : ActionContext(*new ActionContextPrivate(), id, parent) {
    }

    ActionContext::~ActionContext() {
    }

    QString ActionContext::id() const {
        return d_ptr->id;
    }

    QString ActionContext::title() const {
        return d_ptr->id;
    }

    void ActionContext::addRule(const QString &id, const ActionInsertRule &rule) {
        d_ptr->rules[id].append(rule);
    }

    void ActionContext::addRules(const QString &id, const ActionInsertRuleList &rules) {
        d_ptr->rules[id].append(rules);
    }

    void ActionContext::clearRules(const QString &id) {
        d_ptr->rules.remove(id);
    }

    ActionInsertRuleList ActionContext::rules(const QString &id) const {
        auto it = d_ptr->rules.find(id);
        if (it == d_ptr->rules.end()) {
            return {};
        }
        return it.value();
    }

    int ActionContext::rulesCount(const QString &id) const {
        auto it = d_ptr->rules.find(id);
        if (it == d_ptr->rules.end()) {
            return {};
        }
        return it->count();
    }

    void ActionContext::buildMenuBar(QList<ActionItem *> &actionItems, QMenuBar *menuBar) const {
        menuBar->clear();
    }

    void ActionContext::buildMenu(QList<ActionItem *> &actionItems, QMenu *menu) const {
        menu->clear();
    }

    ActionContext::ActionContext(ActionContextPrivate &d, const QString &id, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d_ptr->q_ptr = this;
        d_ptr->init();

        d_ptr->id = id;
    }
}