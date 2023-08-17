//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_MEMORYAUDIOSOURCE_H
#define CHORUSKIT_MEMORYAUDIOSOURCE_H

#include <QScopedPointer>

#include "PositionableAudioSource.h"

class MemoryAudioSourcePrivate;

class MemoryAudioSource: public PositionableAudioSource {
    Q_DECLARE_PRIVATE(MemoryAudioSource)
public:
    explicit MemoryAudioSource(IAudioSampleProvider *buffer = nullptr, bool takeOwnership = false);
    ~MemoryAudioSource();

    IAudioSampleProvider *buffer() const;
    IAudioSampleProvider *setBuffer(IAudioSampleProvider *newBuffer, bool takeOwnership = false);

    qint64 read(const AudioSourceReadData &readData) override;
    qint64 length() const override;

    void setNextReadPosition(qint64 pos) override;
protected:
    explicit MemoryAudioSource(MemoryAudioSourcePrivate &d);
};



#endif // CHORUSKIT_MEMORYAUDIOSOURCE_H
