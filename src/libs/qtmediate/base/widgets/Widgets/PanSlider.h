//
// Created by fluty on 2023/8/7.
//

#ifndef CHORUSKIT_PANSLIDER_H
#define CHORUSKIT_PANSLIDER_H

#include "QSlider"
#include "QMWidgetsGlobal.h"
#include "QMouseEvent"

class PanSliderPrivate;

class QMWIDGETS_EXPORT PanSlider : public QWidget {
    Q_OBJECT

public:
    explicit PanSlider(QWidget *parent = nullptr);
    ~PanSlider();

    void setValue(double value);
    void setDefaultValue(double value);
    void setMax(double max);
    void setMin(double min);
    void setRange(double min, double max);
    void setTrackActiveStartValue(double pos);
    void reset();

signals:
    void valueChanged(double value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    double m_value = 0;
    double m_defaultValue = 0;
    double m_max = 100;
    double m_min = -100;
    double m_trackActiveStartValue = 0;
    bool mouseOnHandle(const QPoint &mousePos);
    bool handleHover = false;
    bool handlePressed = false;
    int actualLeft = 0;
    int actualRight = 0;
    int actualWidth = 0;
    QTimer *timer;
    bool doubleClickLocked = false;

private:
    PanSliderPrivate *d;
};



#endif // CHORUSKIT_PANSLIDER_H
