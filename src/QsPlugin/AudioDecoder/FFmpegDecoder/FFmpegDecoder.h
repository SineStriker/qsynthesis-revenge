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
    FFmpegDecoder(const WaveArguments &args, QObject *parent = nullptr);
    FFmpegDecoder(const QString &fileName, const WaveArguments &args, QObject *parent = nullptr);
    ~FFmpegDecoder();

    bool open();
    void close();

public:
    QString fileName() const;
    void setFileName(const QString &fileName);

    WaveFormat inputFormat() const;
    WaveFormat inputBitsPerSample() const;

public:
    WaveFormat Format() const override;

    void SetPosition(qint64 pos) override;

    qint64 Position() const override;
    qint64 Length() const override;

    int Read(char *buffer, int offset, int count) override;
    int Read(float *buffer, int offset, int count) override;

protected:
    FFmpegDecoder(FFmpegDecoderPrivate &d, const WaveArguments &args, QObject *parent = nullptr);

    QScopedPointer<FFmpegDecoderPrivate> d_ptr;

signals:
};

#endif // FFMPEGDECODER_H
