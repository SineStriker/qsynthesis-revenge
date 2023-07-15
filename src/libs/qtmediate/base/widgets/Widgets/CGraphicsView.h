#ifndef CGRAPHICSVIEW_H
#define CGRAPHICSVIEW_H

#include <QGraphicsView>

#include "QMWidgetsGlobal.h"

class CGraphicsViewPrivate;

class QMWIDGETS_EXPORT CGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    explicit CGraphicsView(QWidget *parent = nullptr);
    explicit CGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
    ~CGraphicsView();

public:
    QRectF viewportRect() const;

    void horizontalTween(int value);
    void verticalTween(int value);

    void setValueX(int value);
    int valueX() const;

    void setValueY(int value);
    int valueY() const;

protected:
    void scrollContentsBy(int dx, int dy) override;
    bool viewportEvent(QEvent *event) override;

private:
    CGraphicsViewPrivate *d;
};

#endif // CGRAPHICSVIEW_H
