//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_MEMORYAUDIOSOURCE_H
#define CHORUSKIT_MEMORYAUDIOSOURCE_H

#include "PositionableAudioSource.h"


class MemoryAudioSource: public PositionableAudioSource {

public:

    explicit MemoryAudioSource(IAudioSampleContainer *buffer, bool takeOwnership = false);
    ~MemoryAudioSource();

    IAudioSampleContainer *buffer() const;
    IAudioSampleContainer *resetBuffer(IAudioSampleContainer *newBuffer, bool takeOwnership = false);

    bool start(int bufferSize, double sampleRate) override;
    bool isStarted() const override;
    int read(const AudioSourceReadData &readData) override;
    void stop() override;
    int length() const override;
    int nextReadPosition() const override;
    void setNextReadPosition(int pos) override;
private:
    IAudioSampleContainer *m_buffer;
    bool m_takeOwnership;
    bool m_isStarted = false;
    int m_position = 0;
};



#endif // CHORUSKIT_MEMORYAUDIOSOURCE_H
