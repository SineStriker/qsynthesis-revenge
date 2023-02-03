#include "CScrollBar.h"

#include "CMenu.h"

#include <QContextMenuEvent>
#include <QPointer>
#include <QStyle>
#include <QStyleOptionSlider>

CScrollBar::CScrollBar(QWidget *parent) : QScrollBar(parent) {
}

CScrollBar::CScrollBar(Qt::Orientation orient, QWidget *parent) : QScrollBar(orient, parent) {
}

CScrollBar::~CScrollBar() {
}

void CScrollBar::replaceScrollBars(QAbstractScrollArea *area) {
    area->setHorizontalScrollBar(new CScrollBar());
    area->setVerticalScrollBar(new CScrollBar());
}

int CScrollBar::pixelPosToRangeValue(int pos) const {
    // Copy from Qt source code
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect gr =
        style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarGroove, this);
    QRect sr =
        style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarSlider, this);
    int sliderMin, sliderMax, sliderLength;

    if (orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
        if (layoutDirection() == Qt::RightToLeft)
            opt.upsideDown = !opt.upsideDown;
    } else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }

    return QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin,
                                           sliderMax - sliderMin, opt.upsideDown);
}

#ifndef QT_NO_CONTEXTMENU
void CScrollBar::contextMenuEvent(QContextMenuEvent *event) {
    return QScrollBar::contextMenuEvent(event);

    if (!style()->styleHint(QStyle::SH_ScrollBar_ContextMenu, nullptr, this)) {
        QAbstractSlider::contextMenuEvent(event);
        return;
    }

    // Copy from Qt source code
#if QT_CONFIG(menu)
    bool horiz = orientation() == Qt::Horizontal;

    QPointer<QMenu> menu = new CMenu(this);
    QAction *actScrollHere = menu->addAction(tr("Scroll here"));
    menu->addSeparator();
    QAction *actScrollTop = menu->addAction(horiz ? tr("Left edge") : tr("Top"));
    QAction *actScrollBottom = menu->addAction(horiz ? tr("Right edge") : tr("Bottom"));
    menu->addSeparator();
    QAction *actPageUp = menu->addAction(horiz ? tr("Page left") : tr("Page up"));
    QAction *actPageDn = menu->addAction(horiz ? tr("Page right") : tr("Page down"));
    menu->addSeparator();
    QAction *actScrollUp = menu->addAction(horiz ? tr("Scroll left") : tr("Scroll up"));
    QAction *actScrollDn = menu->addAction(horiz ? tr("Scroll right") : tr("Scroll down"));
    QAction *actionSelected = menu->exec(event->globalPos());
    if (!actionSelected) {
    } else if (actionSelected == actScrollHere) {
        setValue(pixelPosToRangeValue(horiz ? event->pos().x() : event->pos().y()));
    } else if (actionSelected == actScrollTop) {
        triggerAction(QAbstractSlider::SliderToMinimum);
    } else if (actionSelected == actScrollBottom) {
        triggerAction(QAbstractSlider::SliderToMaximum);
    } else if (actionSelected == actPageUp) {
        triggerAction(QAbstractSlider::SliderPageStepSub);
    } else if (actionSelected == actPageDn) {
        triggerAction(QAbstractSlider::SliderPageStepAdd);
    } else if (actionSelected == actScrollUp) {
        triggerAction(QAbstractSlider::SliderSingleStepSub);
    } else if (actionSelected == actScrollDn) {
        triggerAction(QAbstractSlider::SliderSingleStepAdd);
    }
    delete menu;
#endif // QT_CONFIG(menu)
}
#endif
