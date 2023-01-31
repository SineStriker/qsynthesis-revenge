#ifndef IAUDIODECODER_H
#define IAUDIODECODER_H

#include <QVariantMap>

#include "INamePlugin.h"
#include "NAudio/WaveStream.h"

class QSMEDIA_API IAudioDecoder : public INamePlugin, public NAudio::WaveStream {
    Q_OBJECT
public:
    IAudioDecoder(QObject *parent = nullptr);
    ~IAudioDecoder();

public:
    /**
     * @brief Open audio with common and specific arguments
     *
     * @param args Key-value pairs as open arguments
     */
    virtual bool open(const QVariantMap &args = {}) = 0;
    virtual void close() = 0;

    virtual bool isOpen() const = 0;

    /**
     * @brief The original format of the processed audio
     *
     * @return NAudio::WaveFormat
     */
    virtual NAudio::WaveFormat inFormat() const = 0;
};

#define IAudioDecoder_IID "QSynthesis.Plugin.Media.AudioDecoder"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IAudioDecoder, IAudioDecoder_IID)
QT_END_NAMESPACE

#endif // IAUDIODECODER_H
