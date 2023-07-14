//
// Created by Crs_1 on 2023/7/11.
//

#ifndef CHORUSKIT_TRANSPORTAUDIOSOURCE_H
#define CHORUSKIT_TRANSPORTAUDIOSOURCE_H

#include <QObject>

#include "AudioSource.h"
#include "PositionableAudioSource.h"

class TransportAudioSourcePrivate;
class AudioFormatIO;

class TransportAudioSource: public QObject, public AudioSource {
    Q_OBJECT
#define d_ptr AudioSource::d_ptr
    Q_DECLARE_PRIVATE(TransportAudioSource)
#undef d_ptr
public:
    explicit TransportAudioSource(QObject *parent = nullptr);
    ~TransportAudioSource() override;
    qint64 read(const AudioSourceReadData &readData) override;
    bool open(qint64 bufferSize, double sampleRate) override;
    void close() override;

    void setSource(PositionableAudioSource *src);
    PositionableAudioSource *source() const;

    void play();
    bool isPlaying() const;
    void pause();

    void lock();
    void unlock();

    qint64 position() const;
    void setPosition(qint64 position);
    qint64 length() const;

    QPair<qint64, qint64> loopingRange() const;
    void setLoopingRange(qint64 l, qint64 r);

    void writeToFile(AudioFormatIO *outFile, qint64 length);

signals:
    void positionAboutToChange(qint64 nextPosition);

protected:
    TransportAudioSource(TransportAudioSourcePrivate &d, QObject *parent);
};



#endif // CHORUSKIT_TRANSPORTAUDIOSOURCE_H
