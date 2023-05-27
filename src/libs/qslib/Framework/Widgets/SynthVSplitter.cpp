#include "SynthVSplitter.h"
#include "SynthVSplitter_p.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include <private/qlayout_p.h>
#include <private/qscrollbar_p.h>

class HackScrollBar : public QScrollBar {
public:
    QScrollBarPrivate *d_func() const {
        return reinterpret_cast<QScrollBarPrivate *>(d_ptr.data());
    }
};

namespace QsApi {

    SynthVSplitterPrivate::SynthVSplitterPrivate() {
        area = nullptr;
        blockWidget = nullptr;
    }

    SynthVSplitterPrivate::~SynthVSplitterPrivate() {
    }

    void SynthVSplitterPrivate::init() {
        Q_Q(SynthVSplitter);

        layout = new QBoxLayout(QBoxLayout::BottomToTop);
        layout->setMargin(0);
        layout->setSpacing(0);
        q->setLayout(layout);

        spacer = new SynthVTopSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout->addSpacerItem(spacer);

        connect(spacer, &SynthVTopSpacerItem::geometryChanged, this, &SynthVSplitterPrivate::_q_spacerGeometryChanged);

        hbar = new QScrollBar(Qt::Horizontal, q);
        hbar->setObjectName("hbar");

        vbar = new QScrollBar(Qt::Vertical, q);
        vbar->setObjectName("vbar");

        connect(hbar, &QScrollBar::rangeChanged, this, &SynthVSplitterPrivate::_q_showOrHideScrollBars,
                Qt::QueuedConnection);
        connect(vbar, &QScrollBar::rangeChanged, this, &SynthVSplitterPrivate::_q_showOrHideScrollBars,
                Qt::QueuedConnection);

        hbar->hide();
        vbar->hide();
    }

    bool SynthVSplitterPrivate::correctArea() {
        Q_Q(SynthVSplitter);
        bool res = false;

        auto &w = area;
        if (!w) {
            return false;
        }

        blockWidget = w;
        if (w->pos() != QPoint(0, 0)) {
            w->move(0, 0);
            res = true;
        }
        if (w->size() != q->size()) {
            w->resize(q->size());
            res = true;
        }
        blockWidget = nullptr;

        return res;
    }

    void SynthVSplitterPrivate::copyScrollBarProperties(QScrollBar *oldBar, QScrollBar *scrollBar) {
        scrollBar->setInvertedAppearance(oldBar->invertedAppearance());
        scrollBar->setInvertedControls(oldBar->invertedControls());
        scrollBar->setRange(oldBar->minimum(), oldBar->maximum());
        scrollBar->setOrientation(oldBar->orientation());
        scrollBar->setPageStep(oldBar->pageStep());
        scrollBar->setSingleStep(oldBar->singleStep());
        reinterpret_cast<HackScrollBar *>(scrollBar)->d_func()->viewMayChangeSingleStep =
            reinterpret_cast<HackScrollBar *>(oldBar)->d_func()->viewMayChangeSingleStep;
        scrollBar->setSliderDown(oldBar->isSliderDown());
        scrollBar->setSliderPosition(oldBar->sliderPosition());
        scrollBar->setTracking(oldBar->hasTracking());
        scrollBar->setValue(oldBar->value());
    }

    void SynthVSplitterPrivate::updateScrollBar() {
        if (!area) {
            return;
        }

        bool needH = false;
        bool needV = false;

        updateScrollBarGeometry_helper(&needH, &needV);
        if (needH != needV) {
            updateScrollBarGeometry_helper(&needH, &needV);
        }
    }

    void SynthVSplitterPrivate::updateScrollBarGeometry_helper(bool *needH, bool *needV) {
        bool htransient = hbar->style()->styleHint(QStyle::SH_ScrollBar_Transient, nullptr, hbar);
        bool needh = *needH || ((hbarpolicy != Qt::ScrollBarAlwaysOff) &&
                                ((hbarpolicy == Qt::ScrollBarAlwaysOn && !htransient) ||
                                 ((hbarpolicy == Qt::ScrollBarAsNeeded || htransient) &&
                                  hbar->minimum() < hbar->maximum() && !hbar->sizeHint().isEmpty())));

        bool vtransient = vbar->style()->styleHint(QStyle::SH_ScrollBar_Transient, nullptr, vbar);
        bool needv = *needV || ((vbarpolicy != Qt::ScrollBarAlwaysOff) &&
                                ((vbarpolicy == Qt::ScrollBarAlwaysOn && !vtransient) ||
                                 ((vbarpolicy == Qt::ScrollBarAsNeeded || vtransient) &&
                                  vbar->minimum() < vbar->maximum() && !vbar->sizeHint().isEmpty())));

        QStyleOption opt(0);
        opt.init(area);

        const int hscrollOverlap = hbar->style()->pixelMetric(QStyle::PM_ScrollView_ScrollBarOverlap, &opt, hbar);
        const int vscrollOverlap = vbar->style()->pixelMetric(QStyle::PM_ScrollView_ScrollBarOverlap, &opt, vbar);

        const int hsbExt = hbar->sizeHint().height();
        const int vsbExt = vbar->sizeHint().width();
        const QPoint extPoint(vsbExt, hsbExt);
        const QSize extSize(vsbExt, hsbExt);

        const bool hasCornerWidget = false;

        QPoint cornerOffset((needv && vscrollOverlap == 0) ? vsbExt : 0, (needh && hscrollOverlap == 0) ? hsbExt : 0);
        QRect controlsRect;
        controlsRect = spacer->geometry();
        cornerOffset = QPoint(needv ? vsbExt : 0, needh ? hsbExt : 0);

        // If we have a corner widget and are only showing one scroll bar, we need to move it
        // to make room for the corner widget.
        if (hasCornerWidget && ((needv && vscrollOverlap == 0) || (needh && hscrollOverlap == 0)))
            cornerOffset = extPoint;

        // The corner point is where the scroll bar rects, the corner widget rect and the
        // viewport rect meets.
        const QPoint cornerPoint(controlsRect.bottomRight() + QPoint(1, 1) - cornerOffset);

        // move the scrollbars away from top/left headers
        int vHeaderRight = 0;
        int hHeaderBottom = 0;

        if (needh) {
            QRect horizontalScrollBarRect(QPoint(controlsRect.left() + vHeaderRight, cornerPoint.y()),
                                          QPoint(cornerPoint.x() - 1, controlsRect.bottom()));

            if (!hasCornerWidget && htransient)
                horizontalScrollBarRect.adjust(0, 0, cornerOffset.x(), 0);
            hbar->setGeometry(QStyle::visualRect(opt.direction, opt.rect, horizontalScrollBarRect));
            hbar->raise();
        }

        if (needv) {
            QRect verticalScrollBarRect(QPoint(cornerPoint.x(), controlsRect.top() + hHeaderBottom),
                                        QPoint(controlsRect.right(), cornerPoint.y() - 1));
            if (!hasCornerWidget && vtransient)
                verticalScrollBarRect.adjust(0, 0, 0, cornerOffset.y());
            vbar->setGeometry(QStyle::visualRect(opt.direction, opt.rect, verticalScrollBarRect));
            vbar->raise();
        }

        hbar->setVisible(needh);
        vbar->setVisible(needv);

        *needH = needh;
        *needV = needv;
    }

    void SynthVSplitterPrivate::normalizeItems() {
        bool needFix = false;
        for (int i = 0; i < layout->count() - 1; i += 2) {
            auto item = static_cast<SynthVLayoutItem *>(layout->itemAt(i));
            auto w = item->widget();
            if (!w->isVisible()) {
                continue;
            }

            if (w->height() < item->sizeHint().height()) {
                needFix = true;
                item->delta = w->height() - item->realSizeHint().height();
            }
        }

        if (needFix) {
            layout->invalidate();
        }
    }

    SynthVLayoutItem *SynthVSplitterPrivate::findResizableItem(SynthVSplitterHandle *handle) {
        int index = layout->indexOf(handle);
        if (index < 0) {
            return nullptr;
        }

        int i = index - 1;
        for (; i >= 0; i -= 2) {
            auto item = static_cast<SynthVLayoutItem *>(layout->itemAt(i));
            if (item->widget()->isVisible()) {
                return item;
            }
        }

        return nullptr;
    }

    bool SynthVSplitterPrivate::eventFilter(QObject *obj, QEvent *event) {
        switch (event->type()) {
            case QEvent::Resize:
            case QEvent::Move: {
                auto w = qobject_cast<QWidget *>(obj);
                if (w && w == area && !blockWidget && correctArea()) {
                    return true;
                }
                break;
            }
            default:
                break;
        }
        return QObject::eventFilter(obj, event);
    }

    void SynthVSplitterPrivate::_q_showOrHideScrollBars(int min, int max) {
        Q_UNUSED(min);
        Q_UNUSED(max);
        updateScrollBar();
    }

    void SynthVSplitterPrivate::_q_spacerGeometryChanged(const QRect &rect) {
        Q_UNUSED(rect);
        correctArea();
        updateScrollBar();
    }

    SynthVSplitter::SynthVSplitter(QWidget *parent) : SynthVSplitter(*new SynthVSplitterPrivate(), parent) {
    }

    SynthVSplitter::~SynthVSplitter() {
    }

    void SynthVSplitter::addWidget(QWidget *w) {
        insertWidget(count(), w);
    }

    static SynthVLayoutItem *m_tmpItem = nullptr;

    void SynthVSplitter::insertWidget(int index, QWidget *w) {
        Q_D(SynthVSplitter);

        auto org = QLayoutPrivate::widgetItemFactoryMethod;

        // Exchange method temporarily
        QLayoutPrivate::widgetItemFactoryMethod = [](const QLayout *layout, QWidget *widget) -> QWidgetItem * {
            Q_UNUSED(layout);
            m_tmpItem = new SynthVLayoutItem(widget);
            return m_tmpItem;
        };

        d->layout->insertWidget(index * 2, w);

        QLayoutPrivate::widgetItemFactoryMethod = org;

        auto handle = createHandle(w);
        d->layout->insertWidget(index * 2 + 1, handle);

        m_tmpItem = nullptr;
    }

    void SynthVSplitter::removeWidget(QWidget *w) {
        Q_D(const SynthVSplitter);
        int index = d->layout->indexOf(w);
        if (index < 0)
            return;

        auto handleItem = d->layout->takeAt(index + 1);
        auto handle = handleItem->widget();
        delete handleItem;
        delete handle;

        delete d->layout->takeAt(index);
    }

    QWidget *SynthVSplitter::widget(int index) const {
        Q_D(const SynthVSplitter);
        return d->layout->itemAt(qMin(index * 2, d->layout->count() - 1))->widget();
    }

    int SynthVSplitter::indexOf(QWidget *w) const {
        Q_D(const SynthVSplitter);
        return d->layout->indexOf(w) / 2;
    }

    int SynthVSplitter::count() const {
        Q_D(const SynthVSplitter);
        return (d->layout->count() - 1) / 2;
    }

    bool SynthVSplitter::isEmpty() const {
        Q_D(const SynthVSplitter);
        return d->layout->count() - 1 == 0;
    }

    QSize SynthVSplitter::sizeHint() const {
        Q_D(const SynthVSplitter);
        QSize super = QFrame::sizeHint();
        QSize area = d->area ? d->area->sizeHint() : QSize(0, 0);
        return {qMax(super.width(), area.width()), qMax(super.height(), area.height())};
    }

    QAbstractScrollArea *SynthVSplitter::scrollArea() const {
        Q_D(const SynthVSplitter);
        return d->area;
    }

    QAbstractScrollArea *SynthVSplitter::takeScrollArea() {
        Q_D(SynthVSplitter);
        auto area = d->area;
        if (!area)
            return nullptr;
        d->area = nullptr;

        disconnect(area->horizontalScrollBar(), &QScrollBar::rangeChanged, d->hbar, &QScrollBar::setRange);
        disconnect(area->horizontalScrollBar(), &QScrollBar::valueChanged, d->hbar, &QScrollBar::setValue);
        disconnect(d->hbar, &QScrollBar::valueChanged, area->horizontalScrollBar(), &QScrollBar::setValue);

        disconnect(area->verticalScrollBar(), &QScrollBar::rangeChanged, d->vbar, &QScrollBar::setRange);
        disconnect(area->verticalScrollBar(), &QScrollBar::valueChanged, d->vbar, &QScrollBar::setValue);
        disconnect(d->vbar, &QScrollBar::valueChanged, area->verticalScrollBar(), &QScrollBar::setValue);

        // Restore policy
        area->setHorizontalScrollBarPolicy(d->hbarpolicy);
        area->setVerticalScrollBarPolicy(d->vbarpolicy);

        area->removeEventFilter(d);
        area->setParent(nullptr);

        d->hbar->hide();
        d->vbar->hide();

        return area;
    }

    void SynthVSplitter::setScrollArea(QAbstractScrollArea *area) {
        Q_D(SynthVSplitter);
        delete takeScrollArea();
        d->area = area;

        area->setParent(this);
        area->installEventFilter(d);

        // Save policy
        d->hbarpolicy = area->horizontalScrollBarPolicy();
        d->vbarpolicy = area->verticalScrollBarPolicy();

        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        connect(area->horizontalScrollBar(), &QScrollBar::rangeChanged, d->hbar, &QScrollBar::setRange);
        connect(area->horizontalScrollBar(), &QScrollBar::valueChanged, d->hbar, &QScrollBar::setValue);
        connect(d->hbar, &QScrollBar::valueChanged, area->horizontalScrollBar(), &QScrollBar::setValue);

        connect(area->verticalScrollBar(), &QScrollBar::rangeChanged, d->vbar, &QScrollBar::setRange);
        connect(area->verticalScrollBar(), &QScrollBar::valueChanged, d->vbar, &QScrollBar::setValue);
        connect(d->vbar, &QScrollBar::valueChanged, area->verticalScrollBar(), &QScrollBar::setValue);

        d->copyScrollBarProperties(area->horizontalScrollBar(), d->hbar);
        d->copyScrollBarProperties(area->verticalScrollBar(), d->vbar);

        d->hbar->show();
        d->vbar->show();

        area->show();

        int cnt = d->layout->count();
        for (int i = 0; i < cnt; ++i) {
            auto w = d->layout->itemAt(i)->widget();
            if (w) {
                w->raise();
            }
        }
        d->hbar->raise();
        d->vbar->raise();
    }

    SynthVSplitterHandle *SynthVSplitter::createHandle(QWidget *w) {
        Q_UNUSED(w);
        return new SynthVSplitterHandle(this);
    }

    void SynthVSplitter::resizeEvent(QResizeEvent *event) {
        Q_D(SynthVSplitter);

        d->correctArea();

        QFrame::resizeEvent(event);
    }

    SynthVSplitter::SynthVSplitter(SynthVSplitterPrivate &d, QWidget *parent) : QFrame(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

    // Handle
    SynthVSplitterHandlePrivate::SynthVSplitterHandlePrivate(SynthVSplitter *s, SynthVSplitterHandle *q) : q(q), s(s) {
        m_handleHeight = 6;
        hover = false;
        pressed = false;

        currentItem = nullptr;
    }

    void SynthVSplitterHandlePrivate::init() {
        entity = new QWidget();
        entity->setObjectName("entity");
        entity->setAttribute(Qt::WA_StyledBackground);
        entity->setCursor(Qt::SplitVCursor);
        entity->setFixedHeight(m_handleHeight);

        entity->setParent(q);
        entity->installEventFilter(q);

        q->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    }

    void SynthVSplitterHandlePrivate::correctEntity() {
        if (entity->geometry() != q->rect()) {
            entity->setGeometry(q->rect());
        }
    }


    void SynthVSplitterHandlePrivate::mousePressEvent(QMouseEvent *e) {
        if (e->button() == Qt::LeftButton) {
            currentItem = s->d_func()->findResizableItem(q);
            if (!currentItem) {
                return;
            }

            s->d_func()->normalizeItems();

            orgY = q->y();
            orgDelta = currentItem->delta;
            orgMinDelta = currentItem->widget()->minimumHeight() - currentItem->widget()->height();
            orgMaxDelta = qMin(currentItem->widget()->maximumHeight() - currentItem->widget()->height(),
                               s->d_func()->spacer->geometry().height());
            mouseOffset = q->parentWidget()->mapFromGlobal(e->globalPos()).y();
            pressed = true;

            q->update();
        }
    }

    void SynthVSplitterHandlePrivate::mouseMoveEvent(QMouseEvent *e) {
        if (!(e->buttons() & Qt::LeftButton))
            return;

        if (!currentItem) {
            return;
        }

        int pos = q->parentWidget()->mapFromGlobal(e->globalPos()).y() - mouseOffset;

        auto layout = static_cast<QBoxLayout *>(q->parentWidget()->layout());

        int delta = -pos;

        if (delta > orgMaxDelta)
            delta = orgMaxDelta;
        if (delta < orgMinDelta)
            delta = orgMinDelta;

        currentItem->delta = orgDelta + delta;

        layout->invalidate();
    }

    void SynthVSplitterHandlePrivate::mouseReleaseEvent(QMouseEvent *e) {
        if (e->button() == Qt::LeftButton) {
            currentItem = nullptr;
            pressed = false;
            q->update();
        }
    }

    SynthVSplitterHandle::SynthVSplitterHandle(SynthVSplitter *parent)
        : QFrame(parent), d(new SynthVSplitterHandlePrivate(parent, this)) {
        d->init();
    }

    SynthVSplitterHandle::~SynthVSplitterHandle() {
        delete d;
    }

    int SynthVSplitterHandle::handleHeight() {
        return d->m_handleHeight;
    }

    void SynthVSplitterHandle::setHandleHeight(int h) {
        d->m_handleHeight = h;
        d->entity->setFixedHeight(h);
        updateGeometry();
    }

    QSize SynthVSplitterHandle::sizeHint() const {
        auto super = QFrame::sizeHint();
        return {super.width(), qMax(d->m_handleHeight, super.height())};
    }

    void SynthVSplitterHandle::resizeEvent(QResizeEvent *event) {
        d->correctEntity();
        QWidget::resizeEvent(event);
    }

    bool SynthVSplitterHandle::event(QEvent *event) {
        switch (event->type()) {
            case QEvent::ChildAdded: {
                auto e = static_cast<QChildEvent *>(event);
                auto child = e->child();
                if (child->isWidgetType() && child != d->entity) {
                    d->entity->raise();
                }
                break;
            }
            default:
                break;
        }
        return QFrame::event(event);
    }

    bool SynthVSplitterHandle::eventFilter(QObject *obj, QEvent *event) {
        if (obj == d->entity) {
            switch (event->type()) {
                case QEvent::MouseButtonPress:
                    d->mousePressEvent(static_cast<QMouseEvent *>(event));
                    break;
                case QEvent::MouseMove:
                    d->mouseMoveEvent(static_cast<QMouseEvent *>(event));
                    break;
                case QEvent::MouseButtonRelease:
                    d->mouseReleaseEvent(static_cast<QMouseEvent *>(event));
                    break;
                case QEvent::HoverEnter:
                    d->hover = true;
                    update();
                    break;
                case QEvent::HoverLeave:
                    d->hover = false;
                    update();
                    break;
                case QEvent::Move:
                case QEvent::Resize:
                    d->correctEntity();
                    break;
                default:
                    break;
            }
        }
        return QObject::eventFilter(obj, event);
    }

} // namespace QsApi
