//
// Created by Crs_1 on 2023/5/10.
//

#ifndef CHORUSKIT_VSTAUDIOOUTPUTMANAGER_H
#define CHORUSKIT_VSTAUDIOOUTPUTMANAGER_H



#include "VstAudioOutput.h"
#include <QObject>
class VstAudioOutputManager: public QObject {
    Q_OBJECT
public:
    struct PlaybackParameters {
        double sampleRate;
        qint64 position;
        qint32 bufferSize;
    };

    explicit VstAudioOutputManager(QObject *parent = nullptr);
    VstAudioOutput *output(int index);
    bool vstProcess(const PlaybackParameters *param, bool isPlaying, qint32 outputCount, float *const *const *outputs);
    bool vstProcessInitializer(bool isOffline, double sampleRate);
private:
    quint32 m_sampleRate = 0;
    QList<VstAudioOutput *> m_outputs;
};



#endif // CHORUSKIT_VSTAUDIOOUTPUTMANAGER_H
