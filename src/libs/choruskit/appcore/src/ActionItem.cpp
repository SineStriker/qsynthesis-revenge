#include "ActionItem.h"
#include "ActionItem_p.h"

#include "ActionSystem.h"
#include "ICoreBase.h"

#include <QDebug>

namespace Core {

    static bool m_autoDelete = false;

    ActionItemPrivate::ActionItemPrivate() : q_ptr(nullptr) {
        type = ActionItem::Invalid;
        autoDelete = m_autoDelete;
    }

    ActionItemPrivate::~ActionItemPrivate() {
    }

    void ActionItemPrivate::init() {
    }

    bool ActionItemPrivate::getSpec() {
        spec = ICoreBase::instance()->actionSystem()->action(id);
        if (!spec) {
            qWarning() << "ActionItem: action is not registered to ActionSystem:" << id;
            return false;
        }

        commandName = spec->commandName();

        connect(spec, &ActionSpec::shortcutsChanged, this, &ActionItemPrivate::_q_actionShortcutsChanged);
        if (type == ActionItem::Action) {
            action->setShortcuts(spec->shortcuts());
        }

        return true;
    }

    void ActionItemPrivate::_q_actionShortcutsChanged(const QList<QKeySequence> &shortcuts) {
        if (type == ActionItem::Action) {
            action->setShortcuts(shortcuts);
        }
    }

    ActionItem::ActionItem(const QString &id, QAction *action, QObject *parent)
        : ActionItem(*new ActionItemPrivate(), id, parent) {
        Q_D(ActionItem);

        if (!action) {
            qWarning() << "Core::ActionItem(): trying to wrap null action";
            return;
        }
        d->type = Action;
        d->action = action;

        if (!d->getSpec()) {
            return;
        }
    }

    ActionItem::ActionItem(const QString &id, QActionGroup *actionGroup, QObject *parent)
        : ActionItem(*new ActionItemPrivate(), id, parent) {
        Q_D(ActionItem);

        if (!actionGroup) {
            qWarning() << "Core::ActionItem(): trying to wrap null action group";
            return;
        }
        d->type = ActionGroup;
        d->actionGroup = actionGroup;

        if (!d->getSpec()) {
            return;
        }
    }

    ActionItem::ActionItem(const QString &id, QMenu *menu, QObject *parent)
        : ActionItem(*new ActionItemPrivate(), id, parent) {
        Q_D(ActionItem);

        if (!menu) {
            qWarning() << "Core::ActionItem(): trying to wrap null menu";
            return;
        }
        d->type = Menu;
        d->menu = menu;

        if (!d->getSpec()) {
            return;
        }
    }

    ActionItem::ActionItem(const QString &id, QWidget *widget, QObject *parent)
        : ActionItem(*new ActionItemPrivate(), id, parent) {
        Q_D(ActionItem);

        if (!widget) {
            qWarning() << "Core::ActionItem(): trying to wrap null widget";
            return;
        }
        d->type = Widget;
        d->widget = widget;

        if (!d->getSpec()) {
            return;
        }
    }

    ActionItem::~ActionItem() {
        Q_D(ActionItem);

        if (d->autoDelete) {
            QObject *obj = nullptr;
            switch (d->type) {
                case Action:
                    obj = d->action;
                    break;
                case ActionGroup:
                    obj = d->actionGroup;
                    break;
                case Menu:
                    obj = d->menu;
                    break;
                case Widget:
                    obj = d->widget;
                    break;
                default:
                    break;
            }
            if (obj && !obj->parent()) {
                obj->deleteLater();
            }
        }
    }

    QString ActionItem::id() const {
        Q_D(const ActionItem);
        return d->id;
    }

    ActionItem::Type ActionItem::type() const {
        Q_D(const ActionItem);
        return d->type;
    }

    ActionSpec *ActionItem::spec() const {
        Q_D(const ActionItem);
        return d->spec;
    }

    QAction *ActionItem::action() const {
        Q_D(const ActionItem);
        return d->action;
    }

    QActionGroup *ActionItem::actionGroup() const {
        Q_D(const ActionItem);
        return d->actionGroup;
    }

    QMenu *ActionItem::menu() const {
        Q_D(const ActionItem);
        return d->menu;
    }

    QWidget *ActionItem::widget() const {
        Q_D(const ActionItem);
        return d->widget;
    }

    QIcon ActionItem::icon() const {
        Q_D(const ActionItem);
        QIcon res;
        switch (d->type) {
            case Action:
                res = d->action->icon();
                break;
            case ActionGroup:
                res = d->actionGroup->property("icon").value<QIcon>();
                break;
            case Menu:
                res = d->menu->icon();
                break;
            case Widget:
                res = d->widget->windowIcon();
                break;
            default:
                break;
        }
        return res;
    }
    void ActionItem::setIcon(const QIcon &icon) {
        Q_D(ActionItem);

        switch (d->type) {
            case Action:
                d->action->setIcon(icon);
                break;
            case ActionGroup:
                d->actionGroup->setProperty("icon", icon);
                break;
            case Menu:
                d->menu->setIcon(icon);
                break;
            case Widget:
                d->widget->setWindowIcon(icon);
                break;
            default:
                break;
        }
    }

    QString ActionItem::text() const {
        Q_D(const ActionItem);

        QString res;
        switch (d->type) {
            case Action:
                res = d->action->text();
                break;
            case ActionGroup:
                res = d->actionGroup->property("text").value<QString>();
                break;
            case Menu:
                res = d->menu->title();
                break;
            case Widget:
                res = d->widget->windowTitle();
                break;
            default:
                break;
        }
        return res;
    }

    void ActionItem::setText(const QString &text) {
        Q_D(ActionItem);
        switch (d->type) {
            case Action:
                d->action->setText(text);
                break;
            case ActionGroup:
                d->actionGroup->setProperty("text", text);
                break;
            case Menu:
                d->menu->setTitle(text);
                break;
            case Widget:
                d->widget->setWindowTitle(text);
                break;
            default:
                break;
        }
    }

    bool ActionItem::enabled() const {
        Q_D(const ActionItem);

        bool res = false;
        switch (d->type) {
            case Action:
                res = d->action->isEnabled();
                break;
            case ActionGroup:
                res = d->actionGroup->isEnabled();
                break;
            case Menu:
                res = d->menu->isEnabled();
                break;
            case Widget:
                res = d->widget->isEnabled();
                break;
            default:
                break;
        }
        return res;
    }

    void ActionItem::setEnabled(bool enabled) {
        Q_D(ActionItem);

        switch (d->type) {
            case Action:
                d->action->setEnabled(enabled);
                break;
            case ActionGroup:
                d->actionGroup->setEnabled(enabled);
                break;
            case Menu:
                d->menu->setEnabled(enabled);
                break;
            case Widget:
                d->widget->setEnabled(enabled);
                break;
            default:
                break;
        }
    }

    bool ActionItem::autoDelete() const {
        Q_D(const ActionItem);
        return d->autoDelete;
    }

    void ActionItem::setAutoDelete(bool autoDelete) {
        Q_D(ActionItem);
        d->autoDelete = autoDelete;
    }

    QString ActionItem::commandDescription() const {
        Q_D(const ActionItem);
        return d->commandDesc;
    }

    void ActionItem::setCommandDescription(const QString &originalCommandName) {
        Q_D(ActionItem);
        d->commandDesc = originalCommandName;
    }

    QString ActionItem::commandName() const {
        Q_D(const ActionItem);
        return d->commandName;
    }

    void ActionItem::setCommandName(const QString &commandName) {
        Q_D(ActionItem);
        d->commandName = commandName;
    }

    bool ActionItem::autoDeleteGlobal() {
        return m_autoDelete;
    }

    void ActionItem::setAutoDeleteGlobal(bool autoDelete) {
        m_autoDelete = autoDelete;
    }

    ActionItem::ActionItem(ActionItemPrivate &d, const QString &id, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;

        d.init();
    }

}