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

    void setLevel(double d);
    void setSampleRate(int sampleRate);
    void setBufferSize(int size);
    void initBuffer();

protected:
    void paintEvent(QPaintEvent *event) override;
    double averageLevel = 0;
    int sampleRate = 44100;
    double *bufferPtr;
    int bufferPos = 0;
    int bufferSize = 8;
    QTimer *timer;

private:
    LevelMeterChunkPrivate *d;
    void resetBuffer();
};



#endif // CHORUSKIT_LEVELMETERCHUNK_H
