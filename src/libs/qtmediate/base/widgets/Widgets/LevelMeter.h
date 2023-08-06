//
// Created by fluty on 2023/8/6.
//

#ifndef CHORUSKIT_LEVELMETER_H
#define CHORUSKIT_LEVELMETER_H

#include "QProgressBar"
#include "QWidget"
#include "QMWidgetsGlobal.h"
#include "QPixelSize.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QPushButton"
#include "QSizePolicy"

class LevelMeterPrivate;

class QMWIDGETS_EXPORT LevelMeter : public QWidget {
    Q_OBJECT
public:
    explicit LevelMeter(QWidget *parent = nullptr);
    ~LevelMeter();

    void setLevel(double d);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    LevelMeterPrivate *d;
    bool *clipped;
};



#endif // CHORUSKIT_LEVELMETER_H
