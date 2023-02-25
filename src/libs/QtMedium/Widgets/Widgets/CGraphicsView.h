#ifndef CGRAPHICSVIEW_H
#define CGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QPropertyAnimation>

#include "QMWidgetsGlobal.h"

class QMWIDGETS_API CGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    explicit CGraphicsView(QWidget *parent = nullptr);
    explicit CGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
    ~CGraphicsView();

private:
    void init();

public:
    QRectF viewportRect() const;

    void horizontalTween(int value);
    void verticalTween(int value);

    void setValueX(int value);
    int valueX() const;

    void setValueY(int value);
    int valueY() const;

protected:
    QPropertyAnimation *m_horizontalAnimation;
    QPropertyAnimation *m_verticalAnimation;

    void scrollContentsBy(int dx, int dy) override;
    bool viewportEvent(QEvent *event) override;

signals:
};

#endif // CGRAPHICSVIEW_H
