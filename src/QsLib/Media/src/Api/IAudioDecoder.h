#ifndef IAUDIODECODER_H
#define IAUDIODECODER_H

#include "qsmedia_global.h"

#include "NAudio/WaveStream.h"

#include <QVariantMap>

class QSMEDIA_API IAudioDecoder : public QObject, public WaveStream {
    Q_OBJECT
public:
    IAudioDecoder(QObject *parent = nullptr);
    ~IAudioDecoder();

public:
    virtual bool open(const QVariantMap &args = {}) = 0;
    virtual void close() = 0;

    virtual bool isOpen() const = 0;

    virtual WaveFormat outFormat() const = 0;
};

#define IAudioDecoder_IID "QSynthesis.Plugin.Media.AudioDecoder"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IAudioDecoder, IAudioDecoder_IID)
QT_END_NAMESPACE

#endif // IAUDIODECODER_H
