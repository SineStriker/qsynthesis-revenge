#include "ActionSystem.h"
#include "ActionSystem_p.h"

#include "ActionContext_p.h"

#include "Collections/QMChronSet.h"
#include "Serialization/QMXmlAdaptor.h"

#include <QDebug>
#include <QFile>

namespace Core {

    ActionSpec::ActionSpec() : d(nullptr) {
    }

    bool ActionSpec::isValid() const {
        return d != nullptr;
    }

    QString ActionSpec::id() const {
        return d ? d->id : QString();
    }

    QString ActionSpec::displayName() const {
        return d ? d->displayName : QString();
    }

    void ActionSpec::setDisplayName(const QString &displayName) {
        if (!d)
            return;
        d->displayName = displayName;
        emit d->d->q_ptr->actionDisplayNameChanged(d->id, displayName);
    }

    QString ActionSpec::description() const {
        return d ? d->description : QString();
    }

    void ActionSpec::setDescription(const QString &description) {
        if (!d)
            return;
        d->description = description;
        emit d->d->q_ptr->actionDisplayNameChanged(d->id, description);
    }

    QList<QKeySequence> ActionSpec::shortcuts() const {
        return d ? d->shortcuts : QList<QKeySequence>();
    }

    void ActionSpec::setShortcuts(const QList<QKeySequence> &shortcuts) {
        if (!d)
            return;
        d->shortcuts = shortcuts;
        emit d->d->q_ptr->actionShortcutsChanged(d->id, shortcuts);
    }

    ActionSpec::ActionSpec(ActionSpecPrivate *d) : d(d) {
    }

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

    ActionSpec ActionSystem::addAction(const QString &id) {
        Q_D(ActionSystem);
        if (d->actions.contains(id)) {
            qWarning() << "Core::ActionSystem::addAction(): trying to add duplicated action:" << id;
            return {};
        }
        auto it = d->actions.append(id, {d, id, id, {}, {}});

        emit actionAdded(id);

        return ActionSpec(&(it.value()));
    }

    void ActionSystem::removeAction(const QString &id) {
        Q_D(ActionSystem);
        auto it = d->actions.find(id);
        if (it == d->actions.end()) {
            qWarning() << "Core::ActionSystem::removeAction(): action does not exist:" << id;
            return;
        }
        d->actions.erase(it);

        emit actionRemoved(id);
    }

    ActionSpec ActionSystem::action(const QString &id) {
        Q_D(ActionSystem);
        auto it = d->actions.find(id);
        if (it == d->actions.end()) {
            return {};
        }
        return ActionSpec(&(it.value()));
    }

    QStringList ActionSystem::actionIds() const {
        Q_D(const ActionSystem);
        return d->actions.keys();
    }

    bool ActionSystem::addContext(ActionContext *context) {
        Q_D(ActionSystem);
        if (!context) {
            qWarning() << "Core::ActionSystem::addContext(): trying to add null context";
            return false;
        }
        if (d->contexts.contains(context->id())) {
            qWarning() << "Core::ActionSystem::addContext(): trying to add duplicated context:" << context->id();
            return false;
        }
        context->setParent(this);
        context->d_ptr->setDirty();
        d->contexts.append(context->id(), context);

        return true;
    }

    bool ActionSystem::removeContext(ActionContext *context) {
        if (context == nullptr) {
            qWarning() << "Core::ActionSystem::removeContext(): trying to remove null context";
            return false;
        }
        return removeContext(context->id());
    }

    bool ActionSystem::removeContext(const QString &id) {
        Q_D(ActionSystem);
        auto it = d->contexts.find(id);
        if (it == d->contexts.end()) {
            qWarning() << "Core::ActionSystem::removeContext(): context does not exist:" << id;
            return false;
        }

        auto context = it.value();
        context->setParent(nullptr);
        context->d_ptr->setDirty();
        d->contexts.erase(it);

        return true;
    }

    ActionContext *ActionSystem::context(const QString &id) const {
        Q_D(const ActionSystem);
        return d->contexts.value(id, nullptr);
    }

    QList<ActionContext *> ActionSystem::contexts() const {
        Q_D(const ActionSystem);
        return d->contexts.values();
    }

    QStringList ActionSystem::contextIds() const {
        Q_D(const ActionSystem);
        return d->contexts.keys();
    }

    void ActionSystemPrivate::loadContexts_dfs(const QString &prefix, const QString &parentId,
                                                const QMXmlAdaptorElement *ele, ActionContext *context) {
        Q_Q(ActionSystem);

        const auto &ctx2 = *ele;
        QString id = prefix + ctx2.properties.value("id");
        if (id.isEmpty()) {
            return;
        }

        QList<ActionInsertRule> rules;
        QList<QKeySequence> shortcuts;
        bool isGroup = ctx2.name == "menu" || ctx2.name == "group";
        if (!isGroup) {
            if (ctx2.name != "action") {
                return;
            }

            QString key = ctx2.properties.value("shortcut");
            if (!key.isEmpty()) {
                shortcuts.append(key);
            }
        }

        if (!parentId.isEmpty()) {
            rules.append({parentId, ActionInsertRule::Append});
        }

        QList<QMXmlAdaptorElement *> children;
        for (const auto &ctx_ref3 : qAsConst(ctx2.children)) {
            const auto &ctx3 = *ctx_ref3;
            if (ctx3.name == "rules") {
                if (rules.isEmpty()) {
                    for (const auto &ctx_ref4 : qAsConst(ctx3.children)) {
                        const auto &ctx4 = *ctx_ref4;
                        if (ctx4.name != "rule") {
                            continue;
                        }

                        QString targetId = ctx4.properties.value("id");
                        if (targetId.isEmpty()) {
                            continue;
                        }

                        auto mode = ctx4.properties.value("mode");
                        ActionInsertRule::InsertMode rule = ActionInsertRule::Append;
                        if (!mode.isEmpty()) {
                            if (mode == "prepend" || mode == "unshift") {
                                rule = ActionInsertRule::Unshift;
                            } else if (mode == "insertBehind") {
                                rule = ActionInsertRule::InsertBehind;
                            } else if (mode == "insertFront") {
                                rule = ActionInsertRule::InsertFront;
                            }
                        }

                        rules.append({targetId, rule});
                    }
                }
            } else if (ctx3.name == "shortcuts") {
                if (!isGroup) {
                    for (const auto &ctx_ref4 : qAsConst(ctx3.children)) {
                        const auto &ctx4 = *ctx_ref4;
                        if (ctx4.name != "shortcut" || ctx4.value.isEmpty()) {
                            continue;
                        }
                        shortcuts.append(ctx4.value);
                    }
                }
            } else if (ctx3.name == "content") {
                if (isGroup) {
                    for (const auto &ctx_ref4 : qAsConst(ctx3.children)) {
                        children.append(ctx_ref4.data());
                    }
                }
            } else if (isGroup) {
                children.append(ctx_ref3.data());
            }
        }

        auto action = context->action(id);
        if (!action) {
            action = context->addAction(id, isGroup);
        }
        action.setRules(action.rules() + rules);

        {
            auto it = actions.find(id);
            if (it == actions.end()) {
                auto spec = q->addAction(id);
                spec.setShortcuts(shortcuts);
            } else {
                auto &spec = it.value();
                spec.shortcuts.append(shortcuts);
            }
        }

        for (const auto &child : qAsConst(children)) {
            loadContexts_dfs(prefix, id, child, context);
        }
    }

    QStringList ActionSystem::loadContexts(const QString &filename) {
        Q_D(ActionSystem);

        QMXmlAdaptor file;
        if (!file.load(filename)) {
            qWarning() << "Core::ActionSystem::loadContexts(): load file failed";
            return {};
        }

        const auto &root = file.root;
        if (root.name != "actionSystem") {
            return {};
        }

        QMChronSet<QString> res;
        for (const auto &ctx_ref : qAsConst(root.children)) {
            const auto &ctx = *ctx_ref;
            if (ctx.name != "context") {
                continue;
            }

            QString id = ctx.properties.value("id");
            if (id.isEmpty()) {
                continue;
            }

            ActionContext *context = nullptr;
            if (ctx.properties.value("mode") == "find") {
                context = this->context(id);
            } else if (!this->context(id)) {
                context = new ActionContext(id);
                addContext(context);
            }

            if (!context) {
                continue;
            }

            if (!res.contains(id)) {
                res.append(id);
            }

            QString prefix = ctx.properties.value("prefix");
            for (const auto &ctx_ref2 : qAsConst(ctx.children)) {
                d->loadContexts_dfs(prefix, {}, ctx_ref2.data(), context);
            }
        }

        return res.values();
    }

    QMap<QString, QStringList> ActionSystem::stateCache(const QString &id) {
        Q_D(const ActionSystem);
        return d->stateCaches.value(id, {});
    }

    void ActionSystem::setStateCache(const QString &id, const QMap<QString, QStringList> &state) {
        Q_D(ActionSystem);
        d->stateCaches.insert(id, state);

        auto ctx = context(id);
        if (ctx) {
            ctx->d_ptr->setDirty();
        }
    }

    ActionSystem::ActionSystem(ActionSystemPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        m_instance = this;

        d.q_ptr = this;
        d.init();
    }

}