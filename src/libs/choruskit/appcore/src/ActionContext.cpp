#include "ActionContext.h"
#include "ActionContext_p.h"

#include "ActionSystem.h"

#include <QDebug>

namespace Core {

    ActionContextPrivate::ActionContextPrivate() : q_ptr(nullptr), system(nullptr) {
    }

    void ActionContextPrivate::init() {
    }

    bool ActionContextData::isValid() const {
        return d != nullptr;
    }

    QString ActionContextData::id() const {
        return d ? d->id : QString();
    }
    QList<ActionInsertRule> &ActionContextData::rules() {
        static QList<ActionInsertRule> _list;
        _list.clear();
        return d ? d->rules : _list;
    }
    QSet<ActionItem *> ActionContextData::instances() const {
        return d ? d->instances : QSet<ActionItem *>();
    }

    ActionContextData::ActionContextData(ActionContextDataPrivate *d) : d(d) {
    }

    ActionContext::ActionContext(const QString &id) : ActionContext(*new ActionContextPrivate(), id) {
    }

    ActionContext::~ActionContext() {
        if (d_ptr->system) {
            d_ptr->system->removeContext(this);
        }
    }

    QString ActionContext::id() const {
        return d_ptr->id;
    }

    QString ActionContext::title() const {
        return d_ptr->id;
    }

    ActionContextData ActionContext::addAction(const QString &id) {
        if (d_ptr->actionIndexes.contains(id)) {
            qWarning() << "Core::ActionContext::addAction(): trying to add duplicated action:" << id;
            return ActionContextData(nullptr);
        }
        auto it = d_ptr->actions.insert(d_ptr->actions.end(), {id, {}, {}});
        d_ptr->actionIndexes.insert(id, it);
        return ActionContextData(&(*it));
    }

    void ActionContext::removeAction(const QString &id) {
        auto it = d_ptr->actionIndexes.find(id);
        if (it == d_ptr->actionIndexes.end()) {
            qWarning() << "Core::ActionContext::removeAction(): action does not exist:" << id;
            return;
        }
        d_ptr->actions.erase(it.value());
        d_ptr->actionIndexes.erase(it);
    }

    ActionContextData ActionContext::action(const QString &id) {
        auto it = d_ptr->actionIndexes.find(id);
        if (it == d_ptr->actionIndexes.end()) {
            return ActionContextData(nullptr);
        }
        return ActionContextData(&(*it.value()));
    }

    void ActionContext::buildMenuBar(const QList<ActionItem *> &actionItems, QMenuBar *menuBar) const {
        menuBar->clear();
    }

    void ActionContext::buildMenu(const QList<ActionItem *> &actionItems, QMenu *menu) const {
        menu->clear();
    }

    ActionContext::ActionContext(ActionContextPrivate &d, const QString &id) : d_ptr(&d) {
        d_ptr->q_ptr = this;
        d_ptr->init();

        d_ptr->id = id;
    }
}