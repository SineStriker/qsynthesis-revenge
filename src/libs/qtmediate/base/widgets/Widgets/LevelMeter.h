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
    enum ChannelType {
        Mono,
        Stereo
    };

    explicit LevelMeter(Qt::Orientation orientation, ChannelType channelType = Stereo, QWidget *parent = nullptr);
    ~LevelMeter();


    void readSample(double sample);
    void dismissIndicator();
    void initBuffer(int bufferSize);

protected:
    void paintEvent(QPaintEvent *event) override;
    Qt::Orientation m_orientation;
    ChannelType m_channelType;
    LevelMeterChunk *m_chunk;
    QPushButton *m_button;
    QHBoxLayout *m_hLayout;
    QVBoxLayout *m_vLayout;

private:
    LevelMeterPrivate *d;
};



#endif // CHORUSKIT_LEVELMETER_H
