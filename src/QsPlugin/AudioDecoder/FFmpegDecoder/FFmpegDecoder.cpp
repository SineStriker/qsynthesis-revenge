#include "FFmpegDecoder.h"

#include "private/FFmpegDecoder_p.h"

#include <QDebug>

FFmpegDecoder::FFmpegDecoder(QObject *parent) : FFmpegDecoder(WaveArguments(), parent) {
}

FFmpegDecoder::FFmpegDecoder(const WaveArguments &args, QObject *parent)
    : FFmpegDecoder(*new FFmpegDecoderPrivate(), args, parent) {
}

FFmpegDecoder::FFmpegDecoder(const QString &fileName, const WaveDecoder::WaveArguments &args,
                             QObject *parent)
    : FFmpegDecoder(args, parent) {
    setFileName(fileName);
}

FFmpegDecoder::~FFmpegDecoder() {
}

bool FFmpegDecoder::open() {
    return true;
}

void FFmpegDecoder::close() {
}

QString FFmpegDecoder::fileName() const {
    Q_D(const FFmpegDecoder);
    return d->_fileName;
}

void FFmpegDecoder::setFileName(const QString &fileName) {
    Q_D(FFmpegDecoder);
    if (d->isOpen) {
        qDebug() << "FFmpegDecoder: Change input when decoder is open is prohibited";
        return;
    }
    d->_fileName = fileName;
}

WaveFormat FFmpegDecoder::inputFormat() const {
    Q_D(const FFmpegDecoder);
    return d->_waveFormat;
}

WaveFormat FFmpegDecoder::inputBitsPerSample() const {
    return 0;
}

WaveFormat FFmpegDecoder::Format() const {
    Q_D(const FFmpegDecoder);
    return d->_resampledFormat;
}

void FFmpegDecoder::SetPosition(qint64 pos) {
}

qint64 FFmpegDecoder::Position() const {
    return 0;
}

qint64 FFmpegDecoder::Length() const {
    return 0;
}

int FFmpegDecoder::Read(char *buffer, int offset, int count) {
    return 0;
}

int FFmpegDecoder::Read(float *buffer, int offset, int count) {
    return 0;
}

FFmpegDecoder::FFmpegDecoder(FFmpegDecoderPrivate &d, const WaveDecoder::WaveArguments &args,
                             QObject *parent)
    : IAudioDecoder(args, parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}
