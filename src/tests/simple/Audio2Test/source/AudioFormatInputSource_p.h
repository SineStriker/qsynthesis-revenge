//
// Created by Crs_1 on 2023/7/12.
//

#ifndef CHORUSKIT_AUDIOFORMATINPUTSOURCE_P_H
#define CHORUSKIT_AUDIOFORMATINPUTSOURCE_P_H

#include "AudioFormatInputSource.h"
#include "PositionableAudioSource_p.h"

#include <samplerate.h>

#include <QVector>

class AudioFormatIO;

class AudioFormatInputSourcePrivate: public PositionableAudioSourcePrivate {
    Q_DECLARE_PUBLIC(AudioFormatInputSource);
public:
    AudioFormatIO *io = nullptr;
    bool takeOwnership = false;
    double ratio = 0;
    SRC_STATE *srcState = nullptr;
    QVector<float> inData;
    QVector<float> outData;

    void resizeInDataBuffers(qint64 bufferSize);
    void resizeOutDataBuffers(qint64 bufferSize);

    long fetchInData(float **data);
};

#endif // CHORUSKIT_AUDIOFORMATINPUTSOURCE_P_H
