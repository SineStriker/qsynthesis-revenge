//
// Created by Crs_1 on 2023/7/12.
//

#ifndef CHORUSKIT_AUDIOFORMATINPUTSOURCE_P_H
#define CHORUSKIT_AUDIOFORMATINPUTSOURCE_P_H

#include "AudioFormatInputSource.h"
#include "PositionableAudioSource_p.h"

#include <samplerate.h>

#include <QVector>
#include <QMutex>

class AudioFormatIO;

class AudioFormatInputSourcePrivate: public PositionableAudioSourcePrivate {
    Q_DECLARE_PUBLIC(AudioFormatInputSource);
public:
    AudioFormatIO *io = nullptr;
    double ratio = 0;
    SRC_STATE *srcState = nullptr;

    qint64 inPosition = 0;

    QVector<float> inData;
    QVector<float> outData;

    bool doStereoize = true;

    QMutex mutex;

    void resizeInDataBuffers(qint64 bufferSize);
    void resizeOutDataBuffers(qint64 bufferSize);

    long fetchInData(float **data);

    AudioFormatInputSource::ResampleMode resampleMode = AudioFormatInputSource::SincBestQuality;

};

#endif // CHORUSKIT_AUDIOFORMATINPUTSOURCE_P_H
