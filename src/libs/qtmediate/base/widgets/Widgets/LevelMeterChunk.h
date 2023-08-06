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

protected:
    void paintEvent(QPaintEvent *event) override;
    double level = 0;

private:
    LevelMeterChunkPrivate *d;
};



#endif // CHORUSKIT_LEVELMETERCHUNK_H
