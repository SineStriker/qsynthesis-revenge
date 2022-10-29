#ifndef FFMPEGDECODER_H
#define FFMPEGDECODER_H

#include "Api/IAudioDecoder.h"

class FFmpegDecoderPrivate;

class Q_DECL_EXPORT FFmpegDecoder : public IAudioDecoder {
    Q_OBJECT
    Q_INTERFACES(IAudioDecoder)
    Q_PLUGIN_METADATA(IID IAudioDecoder_IID FILE "plugin.json")

    Q_DECLARE_PRIVATE(FFmpegDecoder)
public:
    explicit FFmpegDecoder(QObject *parent = nullptr);
    ~FFmpegDecoder();

    /*
     *  FileName (*)
     *  SampleRate
     *  SampleFormat
     *  Channels
     */
    bool open(const QVariantMap &args = {}) override;

    void close() override;

public:
    WaveFormat Format() const override;
    WaveFormat outFormat() const override;

    void SetPosition(qint64 pos) override;

    qint64 Position() const override;
    qint64 Length() const override;

    int Read(char *buffer, int offset, int count) override;
    int Read(float *buffer, int offset, int count) override;

protected:
    FFmpegDecoder(FFmpegDecoderPrivate &d, QObject *parent = nullptr);

    QScopedPointer<FFmpegDecoderPrivate> d_ptr;

signals:
};

#endif // FFMPEGDECODER_H
