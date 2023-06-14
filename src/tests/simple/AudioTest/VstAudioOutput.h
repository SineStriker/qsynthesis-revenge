//
// Created by Crs_1 on 2023/5/10.
//

#ifndef CHORUSKIT_VSTAUDIOOUTPUT_H
#define CHORUSKIT_VSTAUDIOOUTPUT_H



#include "AudioBus.h"
#include "AudioTrack.h"
class VstAudioOutput: public QObject, public AudioBus {
    Q_OBJECT
    friend class VstAudioOutputManager;
public:
    explicit VstAudioOutput(QObject *parent = nullptr);
    ~VstAudioOutput();
    bool addSource(IAudioSource *src) override;
    bool removeSource(IAudioSource *source) override;

private:
    AudioTrack *m_track;
};



#endif // CHORUSKIT_VSTAUDIOOUTPUT_H
