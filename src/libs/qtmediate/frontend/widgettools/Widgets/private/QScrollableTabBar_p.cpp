#include "QScrollableTabBar_p.h"
#include "QScrollableTabBar.h"

#include "../QScrollableTabWidget.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QPainter>

#include "QMView.h"

static inline QString id_format() {
    return QString("application/%1.pid%2.data")
        .arg(qApp->applicationName(), QString::number(qApp->applicationPid()));
}

QScrollableTabBar *QScrollableTabBarPrivate::draggedTabBar = nullptr;

QScrollableTabBarPrivate::QScrollableTabBarPrivate() {
}

QScrollableTabBarPrivate::~QScrollableTabBarPrivate() {
}

void QScrollableTabBarPrivate::init() {
    Q_Q(QScrollableTabBar);
    q->setAttribute(Qt::WA_StyledBackground);

    autoHide = false;

    tabs = nullptr;

    current = nullptr;
    previous = nullptr;

    selectionBehaviorOnRemove = QTabBar::SelectPreviousTab;
    draggedIndex = -1;
    needAutoScroll = false;

    entityLayout = new QHBoxLayout();
    entityLayout->setMargin(0);
    entityLayout->setSpacing(0);

    entity = new QWidget(q);
    entity->setObjectName("entity");
    entity->setLayout(entityLayout);
    entity->installEventFilter(q);

    scrollBar = new QScrollBar(Qt::Horizontal, q);
    scrollBar->setObjectName("scroll-bar");
    scrollBar->setContextMenuPolicy(Qt::NoContextMenu);

    scrollBar->installEventFilter(q);

    QObject::connect(scrollBar, &QScrollBar::valueChanged, q,
                     &QScrollableTabBar::_q_scrollBarValueChanged);
    QObject::connect(scrollBar, &QScrollBar::rangeChanged, q,
                     &QScrollableTabBar::_q_scrollBarRangeChanged);

    opacityEffect = new QGraphicsOpacityEffect(scrollBar);
    scrollBar->setGraphicsEffect(opacityEffect);

    opacityTween = new QPropertyAnimation(q, "scrollOpacity");
    opacityTween->setEasingCurve(QEasingCurve::OutCubic);

    timerId = q->startTimer(100);
    underMouse = false;

    updateScroll();
}

void QScrollableTabBarPrivate::layoutScroll() {
    Q_Q(QScrollableTabBar);
    scrollBar->setGeometry(0, q->height() - scrollBar->height(), q->width(), scrollBar->height());
}

void QScrollableTabBarPrivate::updateScroll() {
    Q_Q(QScrollableTabBar);

    int dw = entity->width() - q->width();
    if (dw <= 0) {
        scrollBar->setValue(0);
        scrollBar->hide();

        // Stop animation
        underMouse = false;
        lastResized = QTime();

        opacityTween->stop();
        opacityEffect->setOpacity(0);
    } else {
        scrollBar->show();
        scrollBar->setRange(0, dw);
        scrollBar->setPageStep(dw);
        scrollBar->setSingleStep(qMax(int(dw / 40), 1));
    }
}

void QScrollableTabBarPrivate::runOpacityTween(bool visible) {
    opacityTween->stop();

    double endValue = visible ? 1 : 0;
    double duration = visible ? 200 : 1000;

    opacityTween->setStartValue(opacityEffect->opacity());
    opacityTween->setEndValue(endValue);
    opacityTween->setDuration(qAbs(opacityEffect->opacity() - visible) * duration);

    opacityTween->start();
}

void QScrollableTabBarPrivate::startDrag(QScrollableTabBarTab *tab) {
    if (current != tab) {
        setCurrentTab(tab);
    }

    Q_Q(QScrollableTabBar);

    QDrag *drag = new QDrag(tab);
    QMimeData *mime = new QMimeData();

    // Bind Data
    QVariant var = tab->data();
    if (var.type() == QVariant::String) {
        mime->setData("text/plain", var.toString().toUtf8());
    }
    mime->setData(id_format(), tabs->metaObject()->className());

    QPixmap pixmap = QMView::createDeviceRenderPixmap(q->window()->windowHandle(), tab->size());
    pixmap.fill(Qt::transparent);
    tab->render(&pixmap);

    drag->setMimeData(mime);
    drag->setPixmap(pixmap);
    drag->setHotSpot(tab->mapFromGlobal(QCursor::pos()));

    draggedTabBar = q;

    // Block
    Qt::DropAction res = drag->exec(Qt::ActionMask);

    draggedTabBar = nullptr;

    Q_UNUSED(res)
}

QScrollableTabBarTab *QScrollableTabBarPrivate::tabAtIndex(int index) const {
    auto item = entityLayout->itemAt(index);
    if (!item) {
        return nullptr;
    }
    return qobject_cast<QScrollableTabBarTab *>(item->widget());
}

void QScrollableTabBarPrivate::setCurrentTab(QScrollableTabBarTab *tab) {
    Q_Q(QScrollableTabBar);

    int orgIndex = -1;
    if (current) {
        if (current == tab) {
            goto out;
        }
        orgIndex = entityLayout->indexOf(current);
        current->setSelected(false);
    }
    tab->setSelected(true);
    previous = current;
    current = tab;

out:
    needAutoScroll = true;
    entityLayout->invalidate();

    emit q->currentChanged(entityLayout->indexOf(tab), orgIndex);
}

void QScrollableTabBarPrivate::autoScrollToCurrent() const {
    Q_Q(const QScrollableTabBar);

    auto tab = current;
    if (!tab) {
        return;
    }

    // Move View Port
    if (tab->x() + entity->x() < 0) {
        scrollBar->setValue(tab->x());
    } else if (tab->x() + tab->width() + entity->x() > q->width()) {
        scrollBar->setValue(tab->x() + tab->width() - q->width());
    }
}

void QScrollableTabBarPrivate::updateVisibility() {
    Q_Q(QScrollableTabBar);
    if (autoHide && q->count() == 0) {
        q->hide();
    } else {
        q->show();
    }
}
