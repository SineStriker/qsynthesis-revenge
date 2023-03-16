#include "ActionItem.h"
#include "ActionItem_p.h"

namespace Core {

    ActionItemPrivate::ActionItemPrivate() : q_ptr(nullptr) {
        type = ActionItem::Action;
        action = nullptr;
        actionGroup = nullptr;
        menu = nullptr;
        widget = nullptr;
    }

    void ActionItemPrivate::init() {
    }

    ActionItem::ActionItem(const QString &id, QAction *action) : ActionItem(id, *new ActionItemPrivate()) {
        d_ptr->type = Action;
        d_ptr->action = action;
    }

    ActionItem::ActionItem(const QString &id, QActionGroup *actionGroup) : ActionItem(id, *new ActionItemPrivate()) {
        d_ptr->type = ActionGroup;
        d_ptr->actionGroup = actionGroup;
    }

    ActionItem::ActionItem(const QString &id, QMenu *menu) : ActionItem(id, *new ActionItemPrivate()) {
        d_ptr->type = Menu;
        d_ptr->menu = menu;
    }

    ActionItem::ActionItem(const QString &id, QWidget *widget) : ActionItem(id, *new ActionItemPrivate()) {
        d_ptr->type = Widget;
        d_ptr->widget = widget;
    }

    ActionItem::~ActionItem() {
    }

    QString ActionItem::id() const {
        return d_ptr->id;
    }

    ActionItem::Type ActionItem::type() const {
        return d_ptr->type;
    }

    QAction *ActionItem::action() const {
        return d_ptr->action;
    }

    QActionGroup *ActionItem::actionGroup() const {
        return d_ptr->actionGroup;
    }

    QMenu *ActionItem::menu() const {
        return d_ptr->menu;
    }

    QWidget *ActionItem::widget() const {
        return d_ptr->widget;
    }

    QIcon ActionItem::icon() const {
        QIcon res;
        switch (d_ptr->type) {
            case Action:
                res = d_ptr->action->icon();
                break;
            case ActionGroup:
                res = d_ptr->actionGroup->property("icon").value<QIcon>();
                break;
            case Menu:
                res = d_ptr->menu->icon();
                break;
            case Widget:
                res = d_ptr->widget->windowIcon();
                break;
            default:
                break;
        }
        return res;
    }
    void ActionItem::setIcon(const QIcon &icon) const {
        switch (d_ptr->type) {
            case Action:
                d_ptr->action->setIcon(icon);
                break;
            case ActionGroup:
                d_ptr->actionGroup->setProperty("icon", icon);
                break;
            case Menu:
                d_ptr->menu->setIcon(icon);
                break;
            case Widget:
                d_ptr->widget->setWindowIcon(icon);
                break;
            default:
                break;
        }
    }

    QString ActionItem::text() const {
        QString res;
        switch (d_ptr->type) {
            case Action:
                res = d_ptr->action->text();
                break;
            case ActionGroup:
                res = d_ptr->actionGroup->property("text").value<QString>();
                break;
            case Menu:
                res = d_ptr->menu->title();
                break;
            case Widget:
                res = d_ptr->widget->windowTitle();
                break;
            default:
                break;
        }
        return res;
    }

    void ActionItem::setText(const QString &text) const {
        switch (d_ptr->type) {
            case Action:
                d_ptr->action->setText(text);
                break;
            case ActionGroup:
                d_ptr->actionGroup->setProperty("text", text);
                break;
            case Menu:
                d_ptr->menu->setTitle(text);
                break;
            case Widget:
                d_ptr->widget->setWindowTitle(text);
                break;
            default:
                break;
        }
    }

    ActionItem::ActionItem(const QString &id, ActionItemPrivate &d) : d_ptr(&d) {
        d.q_ptr = this;
        d.init();

        d.id = id;
    }

}