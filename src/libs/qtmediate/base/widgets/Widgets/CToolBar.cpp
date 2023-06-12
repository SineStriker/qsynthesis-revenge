#include "CToolBar.h"
#include "CToolButton.h"

#include <QActionEvent>
#include <QChildEvent>
#include <QDebug>
#include <QLayoutItem>
#include <QMainWindow>
#include <QMetaProperty>
#include <QStyle>
#include <QTimer>
#include <QToolBar>
#include <QWidgetAction>

#include <private/qtoolbar_p.h>
#include <private/qtoolbarlayout_p.h>
#include <private/qwidgetaction_p.h>

QToolBarItem::QToolBarItem(QWidget *widget) : QWidgetItem(widget), action(nullptr), customWidget(false) {
}

bool QToolBarItem::isEmpty() const {
    return action == nullptr || !action->isVisible();
}

class HackToolBarLayout : public QLayout {
public:
    bool expanded, animating;

    void insertAction(int index, QAction *action);
    int indexOf(QAction *action) const;
    using QLayout::indexOf; // bring back the hidden members

private:
    QList<QToolBarItem *> items;

    QToolBarItem *createItem(QAction *action);
};

void HackToolBarLayout::insertAction(int index, QAction *action) {
    index = qMax(0, index);
    index = qMin(items.count(), index);

    QToolBarItem *item = createItem(action);
    if (item) {
        items.insert(index, item);
        invalidate();
    }
}

int HackToolBarLayout::indexOf(QAction *action) const {
    for (int i = 0; i < items.count(); ++i) {
        if (items.at(i)->action == action)
            return i;
    }
    return -1;
}

QToolBarItem *HackToolBarLayout::createItem(QAction *action) {
    bool customWidget = false;
    bool standardButtonWidget = false;
    QWidget *widget = nullptr;
    auto tb = qobject_cast<QToolBar *>(parentWidget());
    if (!tb)
        return (QToolBarItem *) nullptr;

    if (auto widgetAction = qobject_cast<QWidgetAction *>(action)) {
        widget = widgetAction->requestWidget(tb);
        if (widget != nullptr) {
            widget->setAttribute(Qt::WA_LayoutUsesWidgetRect);
            customWidget = true;
        }
    } else if (action->isSeparator()) {
        // Never reach here
    }

    if (!widget) {
        auto button = new CToolButton(tb);
        button->setAutoRaise(true);
        button->setFocusPolicy(Qt::NoFocus);
        button->setIconSize(tb->iconSize());
        button->setToolButtonStyle(tb->toolButtonStyle());
        QObject::connect(tb, SIGNAL(iconSizeChanged(QSize)), button, SLOT(setIconSize(QSize)));
        QObject::connect(tb, SIGNAL(toolButtonStyleChanged(Qt::ToolButtonStyle)), button,
                         SLOT(setToolButtonStyle(Qt::ToolButtonStyle)));
        button->setDefaultAction(action);
        QObject::connect(button, SIGNAL(triggered(QAction *)), tb, SIGNAL(actionTriggered(QAction *)));
        widget = button;
        standardButtonWidget = true;

        // Copy object name and properties
        button->setObjectName(action->objectName());
        for (const auto &key : action->dynamicPropertyNames()) {
            button->setProperty(key, action->property(key));
        }
    }

    widget->hide();
    QToolBarItem *result = new QToolBarItem(widget);
    if (standardButtonWidget)
        result->setAlignment(Qt::AlignJustify);
    result->customWidget = customWidget;
    result->action = action;
    return result;
}

class HackAction : public QAction {
public:
    QWidgetActionPrivate *d_func() const {
        return reinterpret_cast<QWidgetActionPrivate *>(d_ptr.data());
    }
};

CToolBar::CToolBar(const QString &title, QWidget *parent) : QToolBar(title, parent) {
}

CToolBar::CToolBar(QWidget *parent) : QToolBar(parent) {
}

CToolBar::~CToolBar() {
}

void CToolBar::actionEvent(QActionEvent *event) {
    QAction *action = event->action();
    auto d = reinterpret_cast<QToolBarPrivate *>(d_ptr.data());
    auto widgetAction = reinterpret_cast<HackAction *>(qobject_cast<QWidgetAction *>(action));
    auto layout = reinterpret_cast<HackToolBarLayout *>(d->layout);

    switch (event->type()) {
        case QEvent::ActionAdded: {
            if (!action->isSeparator() && !widgetAction) {
                int index = layout->count();
                if (event->before()) {
                    index = layout->indexOf(event->before());
                    Q_ASSERT_X(index != -1, "QToolBar::insertAction", "internal error");
                }
                layout->insertAction(index, action);
                return;
            }
            break;
        }
        default:
            break;
    }
    QToolBar::actionEvent(event);
}