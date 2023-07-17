#include "CGraphicsView.h"


#include <QApplication>
#include <QGraphicsSceneMoveEvent>
#include <QGraphicsSceneResizeEvent>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScrollBar>

#include <QDebug>

class CGraphicsViewPrivate {
public:
    explicit CGraphicsViewPrivate(CGraphicsView *q) : q(q) {
        m_horizontalAnimation = new QPropertyAnimation(q);
        m_horizontalAnimation->setTargetObject(q->horizontalScrollBar());
        m_horizontalAnimation->setPropertyName("value");
        m_horizontalAnimation->setEasingCurve(QEasingCurve::OutCubic);
        m_horizontalAnimation->setDuration(125);

        m_verticalAnimation = new QPropertyAnimation(q);
        m_verticalAnimation->setTargetObject(q->verticalScrollBar());
        m_verticalAnimation->setPropertyName("value");
        m_horizontalAnimation->setEasingCurve(QEasingCurve::OutCubic);
        m_verticalAnimation->setDuration(125);
    }

    CGraphicsView *q;

    QPropertyAnimation *m_horizontalAnimation;
    QPropertyAnimation *m_verticalAnimation;
};

CGraphicsView::CGraphicsView(QWidget *parent) : QGraphicsView(parent), d(new CGraphicsViewPrivate(this)) {
}

CGraphicsView::CGraphicsView(QGraphicsScene *scene, QWidget *parent) : CGraphicsView(parent) {
    setScene(scene);
}

CGraphicsView::~CGraphicsView() {
    delete d;
}

QRectF CGraphicsView::viewportRect() const {
    QRect rect(0, 0, viewport()->width(), viewport()->height());
    QRectF sceneRect = mapToScene(rect).boundingRect();
    return sceneRect;
}

void CGraphicsView::horizontalTween(int value) {
    auto &m_horizontalAnimation = d->m_horizontalAnimation;

    if (value == m_horizontalAnimation->endValue() && m_horizontalAnimation->state() == QPropertyAnimation::Running) {
        return;
    }
    m_horizontalAnimation->stop();
    m_horizontalAnimation->setStartValue(horizontalScrollBar()->value());
    m_horizontalAnimation->setEndValue(value);
    m_horizontalAnimation->start();
}

void CGraphicsView::verticalTween(int value) {
    auto &m_verticalAnimation = d->m_verticalAnimation;

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
