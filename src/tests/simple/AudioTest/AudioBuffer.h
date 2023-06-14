//
// Created by Crs_1 on 2023/5/5.
//

#ifndef CHORUSKIT_AUDIOBUFFER_H
#define CHORUSKIT_AUDIOBUFFER_H

#include <QVector>
#include <QList>

class AudioBuffer: public QVector<float> {};

class AudioBufferList: public QVector<AudioBuffer> {
public:
    AudioBufferList(quint16 channelCount, quint64 size);
    quint64 bufferSize() const;
    void clearBuffer();
    void resize(int asize);
    void resizeBuffer(quint64 size);
};



#endif // CHORUSKIT_AUDIOBUFFER_H
