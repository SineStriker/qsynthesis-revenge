#ifndef FFMPEGDECODER_H
#define FFMPEGDECODER_H

#include "Api/IAudioDecoder.h"

QSAPI_USING_NAMESPACE

class FFmpegDecoderPrivate;

class Q_DECL_EXPORT FFmpegDecoder : public IAudioDecoder {
    Q_OBJECT
    Q_INTERFACES(QsApi::IAudioDecoder)
    Q_PLUGIN_METADATA(IID QsApi_IAudioDecoder_IID FILE "plugin.json")

    Q_DECLARE_PRIVATE(FFmpegDecoder)
public:
    explicit FFmpegDecoder(QObject *parent = nullptr);
    ~FFmpegDecoder();

    /**
     * @brief The filename is required, others has default value determined by original audio
     *
     * @param FileName       Open audio path
     * @param SampleRate     Sample rate to resample
     * @param SampleFormat   Sample format to resample (AV_SAMPLE_FORMAT)
     * @param Channels       Channel count to resample
     */
    bool open(const QVariantMap &args = {}) override;
    void close() override;

    bool isOpen() const override;

    NAudio::WaveFormat inFormat() const override;

public:
    NAudio::WaveFormat Format() const override;

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
