#ifndef IAUDIOPLAYBACK_H
#define IAUDIOPLAYBACK_H

#include <QObject>

#include "IAudioDecoder.h"

#include "qsmedia_global.h"

class IAudioPlaybackPrivate;

class QSMEDIA_API IAudioPlayback : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(IAudioPlayback)
public:
    IAudioPlayback(QObject *parent = nullptr);
    ~IAudioPlayback();

    enum PlaybackState {
        Playing,
        Stopped,
    };

public:
    struct PlaybackArguments {
        int bufferSamples;
        int sampleRate;
        int channels;
        QVariantMap custom;
    };

    bool setup(const PlaybackArguments &args);
    void dispose();

    bool isAvailable() const;

    void setDecoder(IAudioDecoder *decoder);
    bool isReady() const;

    void play();
    void stop();

    PlaybackState state() const;
    bool isPlaying() const;

    virtual QStringList drivers() const;
    virtual QString currentDriver() const;
    virtual bool setDriver(const QString &driver);

    virtual QStringList devices() const;
    virtual QString currentDevice() const;
    virtual bool setDevice(const QString &device);

    int bufferSamples() const;
    int sampleRate() const;
    int channels() const;

protected:
    IAudioPlayback(IAudioPlaybackPrivate &d, QObject *parent = nullptr);

    QScopedPointer<IAudioPlaybackPrivate> d_ptr;

signals:
    void stateChanged();
    void driverChanged();

    void deviceChanged();
    void deviceAdded();
    void deviceRemoved();
};

#define IAudioPlayback_IID "QSynthesis.Plugin.Media.AudioPlayback"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IAudioPlayback, IAudioPlayback_IID)
QT_END_NAMESPACE

#endif // IAUDIOPLAYBACK_H
