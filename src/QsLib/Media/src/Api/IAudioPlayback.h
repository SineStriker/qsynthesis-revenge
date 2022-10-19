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
    void setup(IAudioDecoder *decoder);
    void dispose();

    void play();
    void stop();

    PlaybackState state() const;

protected:
    IAudioPlayback(IAudioPlaybackPrivate &d, QObject *parent = nullptr);

    QScopedPointer<IAudioPlaybackPrivate> d_ptr;
};

#define IAudioPlayback_IID "QSynthesis.Plugin.Media.AudioPlayback"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IAudioPlayback, IAudioPlayback_IID)
QT_END_NAMESPACE

#endif // IAUDIOPLAYBACK_H
