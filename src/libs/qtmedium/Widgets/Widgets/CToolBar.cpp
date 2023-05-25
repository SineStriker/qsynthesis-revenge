#include "CToolBar.h"

#include <QHBoxLayout>
#include <QStyle>
#include <QWidgetAction>

#include <private/qwidgetaction_p.h>

class HackAction : public QAction {
public:
    QWidgetActionPrivate *d_func() const {
        return reinterpret_cast<QWidgetActionPrivate *>(d_ptr.data());
    }
};

class CToolBarPrivate {
public:
    CToolBar *q;

    QSize iconSize;
    QHBoxLayout *layout;

    CToolBarPrivate(CToolBar *q) : q(q) {
        layout = new QHBoxLayout();
        q->setLayout(layout);
    }
};


CToolBar::CToolBar(QWidget *parent) : QFrame(parent), d(new CToolBarPrivate(this)) {
}

CToolBar::~CToolBar() {
    delete d;
}

void CToolBar::clear() {
    auto actions = this->actions();
    for (const auto &action : qAsConst(actions)) {
        removeAction(action);
    }
}

QAction *CToolBar::addSeparator() {
    auto action = new QAction(this);
    action->setSeparator(true);
    addAction(action);
    return action;
}

QAction *CToolBar::insertSeparator(QAction *before) {
    auto action = new QAction(this);
    action->setSeparator(true);
    insertAction(before, action);
    return action;
}

QAction *CToolBar::addWidget(QWidget *widget) {
    auto action = new QWidgetAction(this);
    action->setDefaultWidget(widget);
    reinterpret_cast<HackAction *>(action)->d_func()->autoCreated = true;
    addAction(action);
    return action;
}

QAction *CToolBar::insertWidget(QAction *before, QWidget *widget) {
    auto action = new QWidgetAction(this);
    action->setDefaultWidget(widget);
    reinterpret_cast<HackAction *>(action)->d_func()->autoCreated = true;
    insertAction(before, action);
    return action;
}

QSize CToolBar::iconSize() const {
    return d->iconSize;
}

void CToolBar::setIconSize(const QSize &iconSize) {
    QSize sz = iconSize;
    if (!sz.isValid()) {
        const int metric = style()->pixelMetric(QStyle::PM_ToolBarIconSize, nullptr, this);
        sz = QSize(metric, metric);
    }
    if (d->iconSize != sz) {
        d->iconSize = sz;
        emit iconSizeChanged(d->iconSize);
    }
    // d->layout->invalidate();
}

void CToolBar::actionEvent(QActionEvent *event) {
    QAction *action = event->action();
    auto widgetAction = qobject_cast<QWidgetAction *>(action);

    switch (event->type()) {
        case QEvent::ActionAdded: {
            if (widgetAction != nullptr && reinterpret_cast<HackAction *>(widgetAction)->d_func()->autoCreated)
                widgetAction->setParent(this);

            // int index = d->layout->count();
            // d->layout->insertAction(index, action);
            break;
        }

        case QEvent::ActionChanged:
            d->layout->invalidate();
            break;

        case QEvent::ActionRemoved: {
            //            int index = d->layout->indexOf(action);
            //            if (index != -1) {
            //                delete d->layout->takeAt(index);
            //            }
            break;
        }

        default:
            break;
    }
}
