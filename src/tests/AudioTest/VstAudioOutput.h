//
// Created by Crs_1 on 2023/5/10.
//

#ifndef CHORUSKIT_VSTAUDIOOUTPUT_H
#define CHORUSKIT_VSTAUDIOOUTPUT_H



#include "AudioBus.h"
class VstAudioOutput: public QObject, public AudioBus {
    Q_OBJECT
    friend class VstAudioOutputManager;
public:
    explicit VstAudioOutput(QObject *parent = nullptr);
};



#endif // CHORUSKIT_VSTAUDIOOUTPUT_H
