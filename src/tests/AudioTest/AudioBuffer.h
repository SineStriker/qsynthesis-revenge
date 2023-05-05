//
// Created by Crs_1 on 2023/5/5.
//

#ifndef CHORUSKIT_AUDIOBUFFER_H
#define CHORUSKIT_AUDIOBUFFER_H

#include <QList>

class AudioBuffer {
public:
    AudioBuffer(quint16 channelCount, quint64 size);
    AudioBuffer(const QList<float *>& buffers);
    ~AudioBuffer();
    quint16 channelCount() const;
    float *buffer(quint16 channel);
    const float *constBuffer(quint16 channel) const;
    quint64 size() const;
    void clear();
private:
    QList<float *> m_buffers;
    bool m_isOwnedByInternal;
    quint64 m_size;
};



#endif // CHORUSKIT_AUDIOBUFFER_H
