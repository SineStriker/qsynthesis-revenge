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
#include "LevelMeterChunk.h"

class LevelMeterPrivate;

class QMWIDGETS_EXPORT LevelMeter : public QWidget {
    Q_OBJECT
public:
    explicit LevelMeter(Qt::Orientation orientation, QWidget *parent = nullptr);
    ~LevelMeter();

    void setLevel(double d);
    void initBuffer();

protected:
    void paintEvent(QPaintEvent *event) override;
    Qt::Orientation m_orientation;
    LevelMeterChunk *m_chunk;
    QPushButton *m_button;
    QHBoxLayout *m_hLayout;
    QVBoxLayout *m_vLayout;

private:
    LevelMeterPrivate *d;
    bool *clipped;
};



#endif // CHORUSKIT_LEVELMETER_H
