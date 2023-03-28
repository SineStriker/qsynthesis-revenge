#include "ActionContext.h"
#include "ActionContext_p.h"

#include "ActionItem.h"
#include "ActionSystem.h"

#include <QDebug>

class TreeNode {
public:
    QString id;
    TreeNode *parent;
    QMChronMap<QString, TreeNode *> children;

    ~TreeNode() {
        qDeleteAll(children);
    }
};

namespace Core {

    ActionContextPrivate::ActionContextPrivate() : q_ptr(nullptr), configurable(true), stateDirty(false) {
    }

    void ActionContextPrivate::init() {
    }

    bool ActionContextData::isValid() const {
        return d != nullptr;
    }

    QString ActionContextData::id() const {
        return d ? d->id : QString();
    }

    bool ActionContextData::isGroup() const {
        return d ? d->isGroup : false;
    }

    QList<ActionInsertRule> ActionContextData::rules() const {
        return d ? d->rules : QList<ActionInsertRule>();
    }

    void ActionContextData::setRules(const QList<ActionInsertRule> &rules) {
        if (!d)
            return;
        d->rules = rules;

        d->d->stateDirty = true;
        emit d->d->q_ptr->actionChanged(d->id);
    }

    QList<QKeySequence> ActionContextData::shortcuts() const {
        return d ? d->shortcuts : QList<QKeySequence>();
    }

    void ActionContextData::setShortcuts(const QList<QKeySequence> &shortcuts) {
        if (!d)
            return;
        d->shortcuts = shortcuts;

        d->d->stateDirty = true;
        emit d->d->q_ptr->actionChanged(d->id);
    }

    ActionContextData::ActionContextData(ActionContextDataPrivate *d) : d(d) {
    }

    ActionContext::ActionContext(const QString &id, QObject *parent)
        : ActionContext(*new ActionContextPrivate(), id, parent) {
    }

    ActionContext::~ActionContext() {
        Q_D(ActionContext);
    }

    QString ActionContext::id() const {
        Q_D(const ActionContext);
        return d->id;
    }

    QString ActionContext::title() const {
        Q_D(const ActionContext);
        return d->id;
    }

    ActionContextData ActionContext::addAction(const QString &id, bool isGroup) {
        Q_D(ActionContext);
        if (d->actions.contains(id)) {
            qWarning() << "Core::ActionContext::addAction(): trying to add duplicated action:" << id;
            return ActionContextData(nullptr);
        }
        auto it = d->actions.append(id, {d, id, isGroup, {}, {}});

        d->stateDirty = true;
        emit actionAdded(id);

        return ActionContextData(&(it.value()));
    }

    void ActionContext::removeAction(const QString &id) {
        Q_D(ActionContext);
        auto it = d->actions.find(id);
        if (it == d->actions.end()) {
            qWarning() << "Core::ActionContext::removeAction(): action does not exist:" << id;
            return;
        }
        d->actions.erase(it);

        d->stateDirty = true;
        emit actionRemoved(id);
    }

    ActionContextData ActionContext::action(const QString &id) {
        Q_D(ActionContext);
        auto it = d->actions.find(id);
        if (it == d->actions.end()) {
            return ActionContextData(nullptr);
        }
        return ActionContextData(&(it.value()));
    }

    bool ActionContext::configurable() const {
        Q_D(const ActionContext);
        return d->configurable;
    }

    void ActionContext::setConfigurable(bool configurable) {
        Q_D(ActionContext);
        d->configurable = configurable;
    }

    QMap<QString, QStringList> ActionContext::state() const {
        auto system = qobject_cast<ActionSystem *>(parent());
        if (!system) {
            return {};
        }

        Q_D(const ActionContext);
        if (!d->stateDirty) {
            return d->state;
        }

        auto actions = d->actions;

        QHash<QString, TreeNode *> nodes;
        TreeNode *root = nullptr;

        {
            QSet<TreeNode *> isolatedNodes;

            // Filter cache
            QMap<QString, QStringList> stateCache = system->stateCache(d->id);
            for (auto it = stateCache.begin(); it != stateCache.end(); ++it) {
                const auto &id = it.key();

                TreeNode *node;
                if (id.isEmpty()) {
                    node = root = new TreeNode{{}, nullptr};
                } else {
                    // Find definition
                    auto it2 = d->actions.find(id);
                    if (it2 == d->actions.end() || !it2->isGroup) {
                        continue;
                    }
                    actions.remove(id);

                    auto it3 = nodes.find(id);
                    if (it3 == nodes.end()) {
                        node = new TreeNode{id, nullptr};
                        nodes.insert(id, node);
                        isolatedNodes.insert(node);
                    } else {
                        node = it3.value();
                    }
                }

                for (const auto &childId : qAsConst(it.value())) {
                    // Find definition
                    auto it3 = d->actions.find(childId);
                    if (it3 == d->actions.end()) {
                        continue;
                    }

                    auto it4 = nodes.find(id);
                    if (it4 == nodes.end()) {
                        auto childNode = new TreeNode{childId, node};
                        node->children.append(id, childNode);
                        nodes.insert(childId, childNode);
                    } else {
                        auto &childNode = it4.value();
                        if (!childNode->parent) {
                            childNode->parent = node;
                            node->children.append(childId, node);
                            isolatedNodes.remove(childNode);
                        }
                    }
                }
            }

            qDeleteAll(isolatedNodes);
        }

        if (!root) {
            root = new TreeNode{{}, nullptr};
        }

        int sz;
        do {
            sz = actions.size();
            for (auto it = actions.begin(); it != actions.end();) {
                const auto &id = it->id;
                const auto &rules = it->rules;

                // Insert to root
                if (rules.isEmpty()) {
                    auto node = new TreeNode{id, root};
                    root->children.append(id, node);
                    nodes.insert(id, node);
                    goto done;
                }

                for (const auto &rule : rules) {
                    TreeNode *target;

                    if (rule.id.isEmpty()) {
                        target = root;
                    } else {
                        // Get target definition
                        auto it2 = d->actions.find(rule.id);
                        if (it2 == d->actions.end()) {
                            continue;
                        }

                        auto it3 = nodes.find(rule.id);
                        if (it3 == nodes.end()) {
                            continue;
                        }

                        if (it2->isGroup) {
                            target = it3.value();
                        } else {
                            target = it3.value()->parent;
                            goto non_group;
                        }
                    }

                    switch (rule.direction) {
                        case ActionInsertRule::Append: {
                            auto node = new TreeNode{id, target};
                            target->children.append(id, node);
                            nodes.insert(id, node);
                            goto done;
                            break;
                        }
                        case ActionInsertRule::Unshift: {
                            auto node = new TreeNode{id, target};
                            target->children.prepend(id, node);
                            nodes.insert(id, node);
                            goto done;
                            break;
                        }
                        default:
                            break;
                    }

                    continue;

                non_group:
                    switch (rule.direction) {
                        case ActionInsertRule::Append: {
                            auto node = new TreeNode{id, target};
                            target->children.insert(std::next(target->children.find(rule.id)), id, node);
                            nodes.insert(id, node);
                            goto done;
                            break;
                        }
                        case ActionInsertRule::Unshift: {
                            auto node = new TreeNode{id, target};
                            target->children.insert(target->children.find(rule.id), id, node);
                            nodes.insert(id, node);
                            goto done;
                            break;
                        }
                        default:
                            break;
                    }
                }

                ++it;
                continue;

            done:
                it = actions.erase(it);
            }

        } while (sz != actions.size());

        // Serialize tree
        QMap<QString, QStringList> res;
        {
            QStringList children;
            for (const auto &child : qAsConst(root->children)) {
                children << child->id;
            }
            res.insert(QString(), children);
        }

        for (const auto &node : qAsConst(nodes)) {
            if (node->children.isEmpty()) {
                continue;
            }

            QStringList children;
            for (const auto &child : qAsConst(node->children)) {
                children << child->id;
            }
            res.insert(node->id, children);
        }

        d->state = res;
        d->stateDirty = false;

        delete root;

        return res;
    }

    ActionContext::ActionContext(ActionContextPrivate &d, const QString &id, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.init();

        d.id = id;
    }
}