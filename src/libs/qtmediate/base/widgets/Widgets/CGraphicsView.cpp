#include "CGraphicsView.h"
#include "CScrollBar.h"

#include <QApplication>
#include <QGraphicsSceneMoveEvent>
#include <QGraphicsSceneResizeEvent>
#include <QResizeEvent>

#include <QDebug>

CGraphicsView::CGraphicsView(QWidget *parent) : QGraphicsView(parent) {
    init();
}

CGraphicsView::CGraphicsView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent) {
    init();
}

CGraphicsView::~CGraphicsView() {
}

void CGraphicsView::init() {
    CScrollBar::replaceScrollBars(this);

    m_horizontalAnimation = new QPropertyAnimation(this);
    m_horizontalAnimation->setTargetObject(horizontalScrollBar());
    m_horizontalAnimation->setPropertyName("value");
    m_horizontalAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_horizontalAnimation->setDuration(125);

    m_verticalAnimation = new QPropertyAnimation(this);
    m_verticalAnimation->setTargetObject(verticalScrollBar());
    m_verticalAnimation->setPropertyName("value");
    m_horizontalAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_verticalAnimation->setDuration(125);
}

QRectF CGraphicsView::viewportRect() const {
    QRect rect(0, 0, viewport()->width(), viewport()->height());
    QRectF sceneRect = mapToScene(rect).boundingRect();
    return sceneRect;
}

void CGraphicsView::horizontalTween(int value) {
    if (value == m_horizontalAnimation->endValue() && m_horizontalAnimation->state() == QPropertyAnimation::Running) {
        return;
    }
    m_horizontalAnimation->stop();
    m_horizontalAnimation->setStartValue(horizontalScrollBar()->value());
    m_horizontalAnimation->setEndValue(value);
    m_horizontalAnimation->start();
}

void CGraphicsView::verticalTween(int value) {
    if (value == m_verticalAnimation->endValue() && m_verticalAnimation->state() == QPropertyAnimation::Running) {
        return;
    }
    m_verticalAnimation->stop();
    m_verticalAnimation->setStartValue(verticalScrollBar()->value());
    m_verticalAnimation->setEndValue(value);
    m_verticalAnimation->start();
}

void CGraphicsView::setValueX(int value) {
    horizontalScrollBar()->setValue(value);
}

int CGraphicsView::valueX() const {
    return horizontalScrollBar()->value();
}

void CGraphicsView::setValueY(int value) {
    verticalScrollBar()->setValue(value);
}

int CGraphicsView::valueY() const {
    return verticalScrollBar()->value();
}

void CGraphicsView::scrollContentsBy(int dx, int dy) {
    auto scene = this->scene();
    QRectF rect = viewportRect();

    QGraphicsSceneMoveEvent event;
    event.setOldPos(rect.topLeft() - QPointF(dx, dy));
    event.setNewPos(rect.topLeft());
    event.setWidget(this);

    QGraphicsView::scrollContentsBy(dx, dy);
    if (scene) {
        QApplication::sendEvent(scene, &event);
    }
}

bool CGraphicsView::viewportEvent(QEvent *event) {
    auto scene = this->scene();
    if (event->type() == QEvent::Resize) {
        auto e = static_cast<QResizeEvent *>(event);
        QGraphicsSceneResizeEvent e2;
        e2.setOldSize(e->oldSize());
        e2.setNewSize(e->size());
        e2.setWidget(this);
        if (scene) {
            QApplication::sendEvent(scene, &e2);
        }
    }
    return QGraphicsView::viewportEvent(event);
}
