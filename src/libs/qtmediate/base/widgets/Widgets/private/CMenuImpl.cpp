#include "CMenuImpl.h"

#include <QAccessible>
#include <QDesktopWidget>
#include <QPushButton>
#include <QTimer>
#include <QWhatsThis>
#include <QWidgetAction>

#include <private/qaction_p.h>
#include <private/qeffects_p.h>
#include <private/qguiapplication_p.h>
#include <private/qpushbutton_p.h>
#include <qpa/qplatformtheme.h>

namespace CMenuImpl {

    bool appUseFullScreenForPopup() {
        auto theme = QGuiApplicationPrivate::platformTheme();
        return theme && theme->themeHint(QPlatformTheme::UseFullScreenForPopupMenu).toBool();
    }

}

int QMenuPrivate::scrollerHeight() const {
    Q_Q(const QMenu);
    return qMax(QApplication::globalStrut().height(),
                q->style()->pixelMetric(QStyle::PM_MenuScrollerHeight, nullptr, q));
}

// Windows and KDE allow menus to cover the taskbar, while GNOME and macOS
// don't. Torn-off menus are again different
inline bool QMenuPrivate::useFullScreenForPopup() const {
    return !tornoff && CMenuImpl::appUseFullScreenForPopup();
}

QRect QMenuPrivate::popupGeometry() const {
    Q_Q(const QMenu);
    return useFullScreenForPopup() ? QApplication::desktop()->screenGeometry(q)
                                   : QApplication::desktop()->availableGeometry(q);
}

QRect QMenuPrivate::popupGeometry(int screen) const {
    return useFullScreenForPopup() ? QApplication::desktop()->screenGeometry(screen)
                                   : QApplication::desktop()->availableGeometry(screen);
}

bool QMenuPrivate::isContextMenu() const {
    return qobject_cast<const QMenuBar *>(topCausedWidget()) == nullptr;
}

void QMenuPrivate::updateActionRects() const {
    updateActionRects(popupGeometry());
}

void QMenuPrivate::updateActionRects(const QRect &screen) const {
    Q_Q(const QMenu);
    if (!itemsDirty)
        return;

    q->ensurePolished();

    // let's reinitialize the buffer
    actionRects.resize(actions.count());
    actionRects.fill(QRect());

    int lastVisibleAction = getLastVisibleAction();

    QStyle *style = q->style();
    QStyleOption opt;
    opt.init(q);
    const int hmargin = style->pixelMetric(QStyle::PM_MenuHMargin, &opt, q),
              vmargin = style->pixelMetric(QStyle::PM_MenuVMargin, &opt, q),
              icone = style->pixelMetric(QStyle::PM_SmallIconSize, &opt, q);
    const int fw = style->pixelMetric(QStyle::PM_MenuPanelWidth, &opt, q);
    const int deskFw = style->pixelMetric(QStyle::PM_MenuDesktopFrameWidth, &opt, q);
    const int tearoffHeight = tearoff ? style->pixelMetric(QStyle::PM_MenuTearoffHeight, &opt, q) : 0;
    const int base_y = vmargin + fw + topmargin + (scroll ? scroll->scrollOffset : 0) + tearoffHeight;
    const int column_max_y = screen.height() - 2 * deskFw - (vmargin + bottommargin + fw);
    int max_column_width = 0;
    int y = base_y;

    // for compatibility now - will have to refactor this away
    tabWidth = 0;
    maxIconWidth = 0;
    hasCheckableItems = false;
    ncols = 1;

    for (int i = 0; i < actions.count(); ++i) {
        QAction *action = actions.at(i);
        if (action->isSeparator() || !action->isVisible() || widgetItems.contains(action))
            continue;
        //..and some members
        hasCheckableItems |= action->isCheckable();
        QIcon is = action->icon();
        if (!is.isNull()) {
            maxIconWidth = qMax<uint>(maxIconWidth, icone + 4);
        }
    }

    // calculate size
    QFontMetrics qfm = q->fontMetrics();
    bool previousWasSeparator = true; // this is true to allow removing the leading separators
    const bool contextMenu = isContextMenu();
    for (int i = 0; i <= lastVisibleAction; i++) {
        QAction *action = actions.at(i);
        const bool isSection = action->isSeparator() && (!action->text().isEmpty() || !action->icon().isNull());
        const bool isPlainSeparator = (isSection && !q->style()->styleHint(QStyle::SH_Menu_SupportsSections)) ||
                                      (action->isSeparator() && !isSection);

        if (!action->isVisible() || (collapsibleSeparators && previousWasSeparator && isPlainSeparator))
            continue; // we continue, this action will get an empty QRect

        previousWasSeparator = isPlainSeparator;

        // let the style modify the above size..
        QStyleOptionMenuItem opt;
        q->initStyleOption(&opt, action);
        const QFontMetrics &fm = opt.fontMetrics;

        QSize sz;
        if (QWidget *w = widgetItems.value(action)) {
            sz =
                w->sizeHint().expandedTo(w->minimumSize()).expandedTo(w->minimumSizeHint()).boundedTo(w->maximumSize());
        } else {
            // calc what I think the size is..
            if (action->isSeparator()) {
                sz = QSize(2, 2);
            } else {
                QString s = action->text();
                int t = s.indexOf(QLatin1Char('\t'));
                if (t != -1) {
                    tabWidth = qMax(int(tabWidth), qfm.horizontalAdvance(s.mid(t + 1)));
                    s = s.left(t);
#ifndef QT_NO_SHORTCUT
                } else if (action->isShortcutVisibleInContextMenu() || !contextMenu) {
                    QKeySequence seq = action->shortcut();
                    if (!seq.isEmpty())
                        tabWidth = qMax(int(tabWidth), qfm.horizontalAdvance(seq.toString(QKeySequence::NativeText)));
#endif
                }
                sz.setWidth(fm.boundingRect(QRect(), Qt::TextSingleLine | Qt::TextShowMnemonic, s).width());
                sz.setHeight(qMax(fm.height(), qfm.height()));

                QIcon is = action->icon();
                if (!is.isNull()) {
                    QSize is_sz = QSize(icone, icone);
                    if (is_sz.height() > sz.height())
                        sz.setHeight(is_sz.height());
                }
            }
            sz = style->sizeFromContents(QStyle::CT_MenuItem, &opt, sz, q);
        }


        if (!sz.isEmpty()) {
            max_column_width = qMax(max_column_width, sz.width());
            // wrapping
            if (!scroll && y + sz.height() > column_max_y) {
                ncols++;
                y = base_y;
            } else {
                y += sz.height();
            }
            // update the item
            actionRects[i] = QRect(0, 0, sz.width(), sz.height());
        }
    }

    max_column_width += tabWidth; // finally add in the tab width
    if (!tornoff ||
        (tornoff && scroll)) {    // exclude non-scrollable tear-off menu since the tear-off menu has a fixed size
        const int sfcMargin = style->sizeFromContents(QStyle::CT_Menu, &opt, QApplication::globalStrut(), q).width() -
                              QApplication::globalStrut().width();
        const int min_column_width = q->minimumWidth() - (sfcMargin + leftmargin + rightmargin + 2 * (fw + hmargin));
        max_column_width = qMax(min_column_width, max_column_width);
    }

    // calculate position
    int x = hmargin + fw + leftmargin;
    y = base_y;

    for (int i = 0; i < actions.count(); i++) {
        QRect &rect = actionRects[i];
        if (rect.isNull())
            continue;
        if (!scroll && y + rect.height() > column_max_y) {
            x += max_column_width + hmargin;
            y = base_y;
        }
        rect.translate(x, y);            // move
        rect.setWidth(max_column_width); // uniform width

        // we need to update the widgets geometry
        if (QWidget *widget = widgetItems.value(actions.at(i))) {
            widget->setGeometry(rect);
            widget->setVisible(actions.at(i)->isVisible());
        }

        y += rect.height();
    }
    itemsDirty = 0;
}

int QMenuPrivate::getLastVisibleAction() const {
    // let's try to get the last visible action
    int lastVisibleAction = actions.count() - 1;
    for (; lastVisibleAction >= 0; --lastVisibleAction) {
        const QAction *action = actions.at(lastVisibleAction);
        if (action->isVisible()) {
            // removing trailing separators
            if (action->isSeparator() && collapsibleSeparators)
                continue;
            break;
        }
    }
    return lastVisibleAction;
}


QRect QMenuPrivate::actionRect(QAction *act) const {
    int index = actions.indexOf(act);
    if (index == -1)
        return QRect();

    updateActionRects();

    // we found the action
    return actionRects.at(index);
}

void QMenuPrivate::hideMenu(QMenu *menu) {
    if (!menu)
        return;

    // See two execs below. They may trigger an akward situation
    // when 'menu' (also known as 'q' or 'this' in the many functions
    // around) to become a dangling pointer if the loop manages
    // to execute 'deferred delete' ... posted while executing
    // this same loop. Not good!
    struct Reposter : QObject {
        Reposter(QMenu *menu) : q(menu) {
            Q_ASSERT(q);
            q->installEventFilter(this);
        }
        ~Reposter() {
            if (deleteLater)
                q->deleteLater();
        }
        bool eventFilter(QObject *obj, QEvent *event) override {
            if (obj == q && event->type() == QEvent::DeferredDelete)
                return deleteLater = true;

            return QObject::eventFilter(obj, event);
        }
        QMenu *q = nullptr;
        bool deleteLater = false;
    };

#if QT_CONFIG(effects)
    QSignalBlocker blocker(menu);
    aboutToHide = true;
    // Flash item which is about to trigger (if any).
    if (menu->style()->styleHint(QStyle::SH_Menu_FlashTriggeredItem) && currentAction &&
        currentAction == actionAboutToTrigger && menu->actions().contains(currentAction)) {
        QEventLoop eventLoop;
        QAction *activeAction = currentAction;

        menu->setActiveAction(nullptr);
        const Reposter deleteDeleteLate(menu);
        QTimer::singleShot(60, &eventLoop, SLOT(quit()));
        eventLoop.exec();

        // Select and wait 20 ms.
        menu->setActiveAction(activeAction);
        QTimer::singleShot(20, &eventLoop, SLOT(quit()));
        eventLoop.exec();
    }

    aboutToHide = false;
    blocker.unblock();
#endif // QT_CONFIG(effects)
    if (activeMenu == menu)
        activeMenu = nullptr;
    menu->d_func()->causedPopup.action = nullptr;
    menu->close();
    menu->d_func()->causedPopup.widget = nullptr;
}

// return the top causedPopup.widget that is not a QMenu
QWidget *QMenuPrivate::topCausedWidget() const {
    QWidget *top = causedPopup.widget;
    while (QMenu *m = qobject_cast<QMenu *>(top))
        top = m->d_func()->causedPopup.widget;
    return top;
}

QAction *QMenuPrivate::actionAt(QPoint p) const {
    if (!rect().contains(p)) // sanity check
        return nullptr;

    for (int i = 0; i < actionRects.count(); i++) {
        if (actionRects.at(i).contains(p))
            return actions.at(i);
    }
    return nullptr;
}

void QMenuPrivate::setOverrideMenuAction(QAction *a) {
    Q_Q(QMenu);
    QObject::disconnect(menuAction, SIGNAL(destroyed()), q, SLOT(_q_overrideMenuActionDestroyed()));
    if (a) {
        menuAction = a;
        QObject::connect(a, SIGNAL(destroyed()), q, SLOT(_q_overrideMenuActionDestroyed()));
    } else { // we revert back to the default action created by the QMenu itself
        menuAction = defaultMenuAction;
    }
}

void QMenuPrivate::_q_overrideMenuActionDestroyed() {
    menuAction = defaultMenuAction;
}

void QMenuPrivate::updateLayoutDirection() {
    Q_Q(QMenu);
    // we need to mimic the cause of the popup's layout direction
    // to allow setting it on a mainwindow for example
    // we call setLayoutDirection_helper to not overwrite a user-defined value
    if (!q->testAttribute(Qt::WA_SetLayoutDirection)) {
        if (QWidget *w = causedPopup.widget)
            setLayoutDirection_helper(w->layoutDirection());
        else if (QWidget *w = q->parentWidget())
            setLayoutDirection_helper(w->layoutDirection());
        else
            setLayoutDirection_helper(QGuiApplication::layoutDirection());
    }
}

void QMenuPrivate::drawScroller(QPainter *painter, QMenuPrivate::ScrollerTearOffItem::Type type, const QRect &rect) {
    if (!painter || rect.isEmpty())
        return;

    if (!scroll ||
        !(scroll->scrollFlags & (QMenuPrivate::QMenuScroller::ScrollUp | QMenuPrivate::QMenuScroller::ScrollDown)))
        return;

    Q_Q(QMenu);
    QStyleOptionMenuItem menuOpt;
    menuOpt.initFrom(q);
    menuOpt.state = QStyle::State_None;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.maxIconWidth = 0;
    menuOpt.tabWidth = 0;
    menuOpt.rect = rect;
    menuOpt.menuItemType = QStyleOptionMenuItem::Scroller;
    menuOpt.state |= QStyle::State_Enabled;
    if (type == QMenuPrivate::ScrollerTearOffItem::ScrollDown)
        menuOpt.state |= QStyle::State_DownArrow;

    painter->setClipRect(menuOpt.rect);
    q->style()->drawControl(QStyle::CE_MenuScroller, &menuOpt, painter, q);
}

void QMenuPrivate::drawTearOff(QPainter *painter, const QRect &rect) {
    if (!painter || rect.isEmpty())
        return;

    if (!tearoff)
        return;

    Q_Q(QMenu);
    QStyleOptionMenuItem menuOpt;
    menuOpt.initFrom(q);
    menuOpt.state = QStyle::State_None;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.maxIconWidth = 0;
    menuOpt.tabWidth = 0;
    menuOpt.rect = rect;
    menuOpt.menuItemType = QStyleOptionMenuItem::TearOff;
    if (tearoffHighlighted)
        menuOpt.state |= QStyle::State_Selected;

    painter->setClipRect(menuOpt.rect);
    q->style()->drawControl(QStyle::CE_MenuTearoff, &menuOpt, painter, q);
}

QRect QMenuPrivate::rect() const {
    Q_Q(const QMenu);
    QStyle *style = q->style();
    QStyleOption opt(0);
    opt.init(q);
    const int hmargin = style->pixelMetric(QStyle::PM_MenuHMargin, &opt, q);
    const int vmargin = style->pixelMetric(QStyle::PM_MenuVMargin, &opt, q);
    const int fw = style->pixelMetric(QStyle::PM_MenuPanelWidth, &opt, q);
    return (q->rect().adjusted(hmargin + fw + leftmargin, vmargin + fw + topmargin, -(hmargin + fw + rightmargin),
                               -(vmargin + fw + bottommargin)));
}

QMenuPrivate::ScrollerTearOffItem::ScrollerTearOffItem(QMenuPrivate::ScrollerTearOffItem::Type type,
                                                       QMenuPrivate *mPrivate, QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f), menuPrivate(mPrivate), scrollType(type) {
    if (parent)
        setMouseTracking(parent->style()->styleHint(QStyle::SH_Menu_MouseTracking, nullptr, parent));
}

void QMenuPrivate::ScrollerTearOffItem::paintEvent(QPaintEvent *e) {
    if (!e->rect().intersects(rect()))
        return;

    QPainter p(this);
    QWidget *parent = parentWidget();

    // paint scroll up / down arrows
    menuPrivate->drawScroller(&p, scrollType, QRect(0, 0, width(), menuPrivate->scrollerHeight()));
    // paint the tear off
    if (scrollType == QMenuPrivate::ScrollerTearOffItem::ScrollUp) {
        QRect rect(0, 0, width(), parent->style()->pixelMetric(QStyle::PM_MenuTearoffHeight, nullptr, parent));
        if (menuPrivate->scroll && menuPrivate->scroll->scrollFlags & QMenuPrivate::QMenuScroller::ScrollUp)
            rect.translate(0, menuPrivate->scrollerHeight());
        menuPrivate->drawTearOff(&p, rect);
    }
}

void QMenuPrivate::ScrollerTearOffItem::updateScrollerRects(const QRect &rect) {
    if (rect.isEmpty())
        setVisible(false);
    else {
        setGeometry(rect);
        raise();
        setVisible(true);
    }
}
