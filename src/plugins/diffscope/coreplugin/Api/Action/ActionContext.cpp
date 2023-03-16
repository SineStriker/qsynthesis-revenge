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

    int ActionContext::rulesCount() const {
        return d_ptr->rules.size();
    }

    QList<ActionInsertRule> ActionContext::rules() const {
        return d_ptr->rules;
    }

    QList<ActionInsertRule> &ActionContext::rulesRef() {
        return d_ptr->rules;
    }

    void ActionContext::addRule(const ActionInsertRule &rule) {
        d_ptr->rules.append(rule);
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