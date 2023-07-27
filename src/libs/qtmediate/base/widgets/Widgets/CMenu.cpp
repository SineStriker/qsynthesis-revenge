#include "CMenu.h"

#include <QDebug>
#include <QFontMetrics>

#include <private/qaction_p.h>
#include <private/qkeysequence_p.h>
#include <private/qmenu_p.h>
#include <qiconengine.h>

#include <QMAppExtension.h>
#include <QMCss.h>
#include <QMNamespace.h>

#ifdef Q_OS_WIN
// For Windows drop shadow enhancement
#    include <dwmapi.h>
#endif

#include <private/IconColorImpl.h>


class CMenuPrivate {
public:
    CMenu *q;

    CMenuPrivate(CMenu *q) : q(q) {
        // Initialize Font
        q->setFont(qApp->font());
        // Call Windows enhancement (if applicable)
        initWindowsEnhancement();
    }

    void updateActionStats() {
        for (const auto &action : q->actions()) {
            if (!action->icon().isNull()) {
                q->setProperty("stats", "icon");
                goto out;
            }
        }

        for (const auto &action : q->actions()) {
            if (action->isCheckable()) {
                q->setProperty("stats", "checkable");
                goto out;
            }
        }

        q->setProperty("stats", "trivial");

    out:
        q->style()->polish(q);
    }

private:
    void initWindowsEnhancement() {
#ifdef Q_OS_WIN
        // Disable Qt drop shadow attribute in order to remove CS_DROPSHADOW
        q->setWindowFlag(Qt::NoDropShadowWindowHint, true);
        // Enable DWM shadow for popup
        m_winEnhanceTrigger = QObject::connect(q, &CMenu::aboutToShow, [&]() {
            constexpr int mgn = 1;
            // Constants defined to make older Windows SDK happy
            constexpr int DWMWA_USE_IMMERSIVE_DARK_MODE_ = 20;
            constexpr int DWMWA_WINDOW_CORNER_PREFERENCE_ = 33;
            DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
            /*DWM_WINDOW_CORNER_PREFERENCE*/INT dwcp = /*DWMWCP_ROUNDSMALL*/3;
            UINT dark = 1;
            MARGINS margins = {mgn, mgn, mgn, mgn};
            Q_ASSERT(this->q->winId());
            // Let DWM compose non-client area
            DwmSetWindowAttribute(reinterpret_cast<HWND>(this->q->winId()), DWMWA_NCRENDERING_POLICY, &ncrp,
                                  sizeof(ncrp));
            // Allow extending frame into popup, this way CMenu will have better drop shadow
            DwmExtendFrameIntoClientArea(reinterpret_cast<HWND>(this->q->winId()), &margins);
            // This undocumented API call is required, since with shadow composition, Qt cannot keep up drawing the
            // contents 1 frame too late when the popup window was shown. Without this call the 1 frame will be
            // white which causes flicker.
            // Workaround taken from https://github.com/AvaloniaUI/Avalonia/issues/8316#issuecomment-1166417480
            DwmSetWindowAttribute(reinterpret_cast<HWND>(this->q->winId()), DWMWA_USE_IMMERSIVE_DARK_MODE_, &dark,
                                  sizeof(dark));
            // This round corner settings only works for Windows 11, sets round corner to small so it doesn't look
            // too off
            DwmSetWindowAttribute(reinterpret_cast<HWND>(this->q->winId()), DWMWA_WINDOW_CORNER_PREFERENCE_, &dwcp,
                                  sizeof(dwcp));
            // Disconnect this connection, don't run again
            QObject::disconnect(m_winEnhanceTrigger);
        });
    }
    QMetaObject::Connection m_winEnhanceTrigger;
#else
    }
#endif
};

CMenu::CMenu(QWidget *parent) : QMenu(parent), d(new CMenuPrivate(this)) {
}

CMenu::CMenu(const QString &title, QWidget *parent) : CMenu(parent) {
    setTitle(title);
}

CMenu::~CMenu() {
    delete d;
}

bool CMenu::event(QEvent *event) {
    switch (event->type()) {
        case QEvent::ActionAdded:
        case QEvent::ActionChanged:
        case QEvent::ActionRemoved:
            d->updateActionStats();
            break;
        default:
            break;
    }
    return QMenu::event(event);
}

void CMenu::paintEvent(QPaintEvent *event) {
    auto d = reinterpret_cast<QMenuPrivate *>(d_ptr.data());

    d->updateActionRects();
    QPainter p(this);
    QRegion emptyArea = QRegion(rect());

    QStyleOptionMenuItem menuOpt;
    menuOpt.initFrom(this);
    menuOpt.state = QStyle::State_None;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.maxIconWidth = 0;
    menuOpt.tabWidth = 0;

    style()->drawPrimitive(QStyle::PE_PanelMenu, &menuOpt, &p, this);

    // calculate the scroll up / down rect
    const int fw = style()->pixelMetric(QStyle::PM_MenuPanelWidth, nullptr, this);
    const int hmargin = style()->pixelMetric(QStyle::PM_MenuHMargin, nullptr, this);
    const int vmargin = style()->pixelMetric(QStyle::PM_MenuVMargin, nullptr, this);

    QRect scrollUpRect, scrollDownRect;
    const int leftmargin = fw + hmargin + d->leftmargin;
    const int topmargin = fw + vmargin + d->topmargin;
    const int bottommargin = fw + vmargin + d->bottommargin;
    const int contentWidth = width() - (fw + hmargin) * 2 - d->leftmargin - d->rightmargin;
    if (d->scroll) {
        if (d->scroll->scrollFlags & QMenuPrivate::QMenuScroller::ScrollUp)
            scrollUpRect.setRect(leftmargin, topmargin, contentWidth, d->scrollerHeight());

        if (d->scroll->scrollFlags & QMenuPrivate::QMenuScroller::ScrollDown)
            scrollDownRect.setRect(leftmargin, height() - d->scrollerHeight() - bottommargin, contentWidth,
                                   d->scrollerHeight());
    }

    // calculate the tear off rect
    QRect tearOffRect;
    if (d->tearoff) {
        tearOffRect.setRect(leftmargin, topmargin, contentWidth,
                            style()->pixelMetric(QStyle::PM_MenuTearoffHeight, nullptr, this));
        if (d->scroll && d->scroll->scrollFlags & QMenuPrivate::QMenuScroller::ScrollUp)
            tearOffRect.translate(0, d->scrollerHeight());
    }

    // draw the items that need updating..
    QRect scrollUpTearOffRect = scrollUpRect.united(tearOffRect);
    for (int i = 0; i < d->actions.count(); ++i) {
        QAction *action = d->actions.at(i);
        QRect actionRect = d->actionRects.at(i);
        if (!event->rect().intersects(actionRect) || d->widgetItems.value(action))
            continue;
        // set the clip region to be extra safe (and adjust for the scrollers)
        emptyArea -= QRegion(actionRect);

        QRect adjustedActionRect = actionRect;
        if (!scrollUpTearOffRect.isEmpty() && adjustedActionRect.bottom() <= scrollUpTearOffRect.top())
            continue;

        if (!scrollDownRect.isEmpty() && adjustedActionRect.top() >= scrollDownRect.bottom())
            continue;

        if (adjustedActionRect.intersects(scrollUpTearOffRect)) {
            if (adjustedActionRect.bottom() <= scrollUpTearOffRect.bottom())
                continue;
            else
                adjustedActionRect.setTop(scrollUpTearOffRect.bottom() + 1);
        }

        if (adjustedActionRect.intersects(scrollDownRect)) {
            if (adjustedActionRect.top() >= scrollDownRect.top())
                continue;
            else
                adjustedActionRect.setBottom(scrollDownRect.top() - 1);
        }

        QRegion adjustedActionReg(adjustedActionRect);
        p.setClipRegion(adjustedActionReg);

        QStyleOptionMenuItem opt;
        initStyleOption(&opt, action);
        opt.rect = actionRect;


        if (!opt.icon.isNull()) {
            IconColorImpl::correctIconStateAndColor(
                opt.icon,
                //
                (opt.state & QStyle::State_Enabled)
                    ? ((opt.state & QStyle::State_Selected) ? QM::CS_Hover : QM::CS_Normal)
                    : QM::CS_Disabled,
                //
                IconColorImpl::defaultSalt(this),
                //
                [this, &opt]() mutable -> QString {
                    QString text = opt.text;
                    opt.text = QChar(0x25A0);

                    QPen pen;
                    IconColorImpl::getTextColor(pen, opt.rect.size(), [&](QPainter *painter) {
                        style()->drawControl(QStyle::CE_MenuItem, &opt, painter, this); //
                    });

                    opt.text = text;

                    return QMCss::ColorToCssString(pen.color());
                });
        }

        style()->drawControl(QStyle::CE_MenuItem, &opt, &p, this);
    }

    emptyArea -= QRegion(scrollUpTearOffRect);
    emptyArea -= QRegion(scrollDownRect);

    if (d->scrollUpTearOffItem || d->scrollDownItem) {
        if (d->scrollUpTearOffItem)
            d->scrollUpTearOffItem->updateScrollerRects(scrollUpTearOffRect);
        if (d->scrollDownItem)
            d->scrollDownItem->updateScrollerRects(scrollDownRect);
    } else {
        // paint scroll up /down
        d->drawScroller(&p, QMenuPrivate::ScrollerTearOffItem::ScrollUp, scrollUpRect);
        d->drawScroller(&p, QMenuPrivate::ScrollerTearOffItem::ScrollDown, scrollDownRect);
        // paint the tear off..
        d->drawTearOff(&p, tearOffRect);
    }

    // draw border
    if (fw) {
        QRegion borderReg;
        borderReg += QRect(0, 0, fw, height());            // left
        borderReg += QRect(width() - fw, 0, fw, height()); // right
        borderReg += QRect(0, 0, width(), fw);             // top
        borderReg += QRect(0, height() - fw, width(), fw); // bottom
        p.setClipRegion(borderReg);
        emptyArea -= borderReg;
        QStyleOptionFrame frame;
        frame.rect = rect();
        frame.palette = palette();
        frame.state = QStyle::State_None;
        frame.lineWidth = style()->pixelMetric(QStyle::PM_MenuPanelWidth, &frame);
        frame.midLineWidth = 0;
        style()->drawPrimitive(QStyle::PE_FrameMenu, &frame, &p, this);
    }

    // finally the rest of the spaces
    p.setClipRegion(emptyArea);
    menuOpt.state = QStyle::State_None;
    menuOpt.menuItemType = QStyleOptionMenuItem::EmptyArea;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.rect = rect();
    menuOpt.menuRect = rect();

    style()->drawControl(QStyle::CE_MenuEmptyArea, &menuOpt, &p, this);
}

void CMenu::initStyleOption(QStyleOptionMenuItem *option, const QAction *action) const {
    if (!option || !action)
        return;

    bool mouseDown;
    // Avoid using QMenuPrivate::mouseDown
    {
        QStyleOptionMenuItem testOption;
        QMenu::initStyleOption(&testOption, action);
        mouseDown = testOption.state & QStyle::State_Sunken;
    }

    auto d = reinterpret_cast<QMenuPrivate *>(d_ptr.data());

    option->initFrom(this);
    option->palette = palette();
    option->state = QStyle::State_None;

    if (window()->isActiveWindow())
        option->state |= QStyle::State_Active;
    if (isEnabled() && action->isEnabled() && (!action->menu() || action->menu()->isEnabled()))
        option->state |= QStyle::State_Enabled;
    else
        option->palette.setCurrentColorGroup(QPalette::Disabled);

    option->font = action->font().resolve(font());
    option->fontMetrics = QFontMetrics(option->font);

    if (d->currentAction && d->currentAction == action && !d->currentAction->isSeparator()) {
        option->state |= QStyle::State_Selected | (mouseDown ? QStyle::State_Sunken : QStyle::State_None);
    }

    option->menuHasCheckableItems = d->hasCheckableItems;
    if (!action->isCheckable()) {
        option->checkType = QStyleOptionMenuItem::NotCheckable;
    } else {
        option->checkType = (action->actionGroup() && action->actionGroup()->isExclusive())
                                ? QStyleOptionMenuItem::Exclusive
                                : QStyleOptionMenuItem::NonExclusive;
        option->checked = action->isChecked();
    }
    if (action->menu()) {
        option->menuItemType = QStyleOptionMenuItem::SubMenu;
    } else if (action->isSeparator()) {
        option->menuItemType = QStyleOptionMenuItem::Separator;
    } else if (d->defaultAction == action) {
        option->menuItemType = QStyleOptionMenuItem::DefaultItem;
    } else {
        option->menuItemType = QStyleOptionMenuItem::Normal;
    }
    if (action->isIconVisibleInMenu()) {
        option->icon = action->icon();
    }

    QString textAndAccel = action->text();
    int tabWidth = d->tabWidth;

    // Two patches:
    // 1. Change key sequence separator from "," to " "
    // 2. Change tab width to right align shortcut text

#ifndef QT_NO_SHORTCUT
    if ((action->isShortcutVisibleInContextMenu() || !d->isContextMenu()) &&
        textAndAccel.indexOf(QLatin1Char('\t')) == -1) {
        QKeySequence seq = action->shortcut();
        if (!seq.isEmpty()) {
            QString seqText = seq.toString(QKeySequence::NativeText);
            seqText.replace(", ", " ");
            textAndAccel += QLatin1Char('\t') + seqText;
            tabWidth = QFontMetrics(font()).horizontalAdvance(seqText);
        }
    }
#endif
    option->text = textAndAccel;
    option->tabWidth = tabWidth;
    option->maxIconWidth = d->maxIconWidth;
    option->menuRect = rect();
}
