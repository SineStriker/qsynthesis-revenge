//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIODEVICE_H
#define CHORUSKIT_AUDIODEVICE_H

#include "AudioDeviceCallback.h"
#include "utils/IAudioStream.h"
#include "utils/IErrorStringProvider.h"
#include "utils/INameProvider.h"

#include <QObject>

class AudioDevicePrivate;
class AudioDriver;

/* abstract */ class AudioDevice: public QObject, public IAudioStream, public INameProvider, public IErrorStringProvider {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AudioDevice)
    friend class AudioDriver;
public:
    virtual ~AudioDevice();

    AudioDriver *driver() const;

    int channelCount() const;
    int activeChannelCount() const;
    void setActiveChannelCount(int num);

    QList<qint64> availableBufferSizes() const;
    qint64 preferredBufferSize() const;

    QList<double> availableSampleRates() const;
    double preferredSampleRate() const;

    virtual bool start(AudioDeviceCallback *audioDeviceCallback);
    bool isStarted() const;
    virtual void stop();

signals:


protected:
    explicit AudioDevice(QObject *parent = nullptr);
    AudioDevice(AudioDevicePrivate &d, QObject *parent);
    QScopedPointer<AudioDevicePrivate> d_ptr;

    void setDriver(AudioDriver *driver);

    void setChannelCount(int channelCount);

    void setAvailableBufferSizes(const QList<qint64> &bufferSizes);
    void setPreferredBufferSize(qint64 bufferSize);

    void setAvailableSampleRates(const QList<double> &sampleRates);
    void setPreferredSampleRate(double sampleRate);

};



#endif // CHORUSKIT_AUDIODEVICE_H
