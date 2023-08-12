//
// Created by fluty on 2023/8/6.
//

#ifndef CHORUSKIT_LEVELMETERCHUNK_H
#define CHORUSKIT_LEVELMETERCHUNK_H

#include "QWidget"
#include "QMWidgetsGlobal.h"

class LevelMeterChunkPrivate;

class QMWIDGETS_EXPORT LevelMeterChunk : public QWidget {
    Q_OBJECT

public:
    explicit LevelMeterChunk(QWidget *parent = nullptr);
    ~LevelMeterChunk();

    void readSample(double sample);
    void setOrientation(Qt:: Orientation orientation);
//    void setSampleRate(int sampleRate);
    void setBufferSize(int size);
    void initBuffer(int bufferSize);

protected:
    void paintEvent(QPaintEvent *event) override;

    Qt::Orientation m_orientation;
    QColor m_colorBackground = QColor(217, 217, 217);
    QColor m_colorSafe = QColor(112, 156, 255);
    QColor m_colorWarn = QColor(255, 175, 95);
    QColor m_colorCritical = QColor(255, 124, 128);
    const double m_safeThreshold = 0.707946; //-3 dB
    const double m_warnThreshold = 0.891251; //-1 dB
    double averageLevel = 0;
//    int sampleRate = 44100;
    double *bufferPtr;
    int bufferPos = 0;
    int m_bufferSize = 8;
    QTimer *timer;

private:
    LevelMeterChunkPrivate *d;
    void resetBuffer();
};



#endif // CHORUSKIT_LEVELMETERCHUNK_H
