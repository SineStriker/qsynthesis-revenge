#ifndef NONESCROLLAREA_H
#define NONESCROLLAREA_H

#include <QFrame>

#include "QsIntegrateGlobal.h"

class QSINTEGRATE_API NoneScrollArea : public QFrame {
    Q_OBJECT
public:
    explicit NoneScrollArea(QWidget *parent = nullptr);
    virtual ~NoneScrollArea();

public:
    void setWidget(QWidget *widget);
    QWidget *widget() const;
    QWidget *takeWidget();

    void setPercentageX(double value);
    void setPercentageY(double value);

    double percentageX();
    double percentageY();

    void setValueX(int value);
    int valueX() const;

    void setValueY(int value);
    int valueY() const;

protected:
    QWidget *m_widget;

    void resizeEvent(QResizeEvent *event) override;

signals:
    void resized(QResizeEvent *);
};

#endif // NONESCROLLAREA_H
