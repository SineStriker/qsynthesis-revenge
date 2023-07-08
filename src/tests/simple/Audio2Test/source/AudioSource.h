//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_AUDIOSOURCE_H
#define CHORUSKIT_AUDIOSOURCE_H

class IAudioSampleContainer;

class AudioSourceReadData {
public:
    AudioSourceReadData(IAudioSampleContainer *buffer);
    AudioSourceReadData(IAudioSampleContainer *buffer, int startPos, int length);
    IAudioSampleContainer *buffer;
    int startPos;
    int length;
};

class AudioSource {

public:
    virtual ~AudioSource() = default;
    virtual bool start(int bufferSize, double sampleRate) = 0;
    virtual bool isStarted() const = 0;
    virtual int read(const AudioSourceReadData &readData) = 0;
    virtual void stop() = 0;
};



#endif // CHORUSKIT_AUDIOSOURCE_H
