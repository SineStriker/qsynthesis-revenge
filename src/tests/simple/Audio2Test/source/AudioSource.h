//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_AUDIOSOURCE_H
#define CHORUSKIT_AUDIOSOURCE_H

#include <QScopedPointer>

#include "buffer/IAudioSampleContainer.h"

class AudioSourceReadData {
public:
    AudioSourceReadData(IAudioSampleContainer *buffer); //implicit use IAudioSampleContainer
    AudioSourceReadData(IAudioSampleContainer *buffer, int startPos, int length);
    IAudioSampleContainer *buffer;
    int startPos;
    int length;
};

class AudioSourcePrivate;

class AudioSource {
    Q_DECLARE_PRIVATE(AudioSource)
public:
    AudioSource();
    virtual ~AudioSource();
    virtual bool start(int bufferSize, double sampleRate);
    bool isStarted() const;
    int bufferSize() const;
    double sampleRate() const;
    virtual int read(const AudioSourceReadData &readData) = 0;
    virtual void stop();
protected:
    AudioSource(AudioSourcePrivate &d);
    QScopedPointer<AudioSourcePrivate> d_ptr;
};



#endif // CHORUSKIT_AUDIOSOURCE_H
