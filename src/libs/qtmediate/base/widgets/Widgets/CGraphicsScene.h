#ifndef CGRAPHICSSCENE_H
#define CGRAPHICSSCENE_H

#include <QEnterEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMoveEvent>

#include "QMWidgetsGlobal.h"

class QMWIDGETS_EXPORT CGraphicsScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit CGraphicsScene(QObject *parent = nullptr);
    CGraphicsScene(const QRectF &sceneRect, QObject *parent = nullptr);
    CGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = nullptr);
    ~CGraphicsScene();

protected:
    bool event(QEvent *event) override;

    virtual void viewMoveEvent(QGraphicsSceneMoveEvent *event);
    virtual void viewResizeEvent(QGraphicsSceneResizeEvent *event);

    virtual void enterEvent(QEnterEvent *event);
    virtual void leaveEvent(QEvent *event);
};

#endif // CGRAPHICSSCENE_H
