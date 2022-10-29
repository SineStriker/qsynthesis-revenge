#include "FFmpegDecoder_p.h"

FFmpegDecoderPrivate::FFmpegDecoderPrivate() {
}

FFmpegDecoderPrivate::~FFmpegDecoderPrivate() {
}

void FFmpegDecoderPrivate::init() {
    isOpen = false;

    clear();
}

void FFmpegDecoderPrivate::clear() {
    _formatContext = nullptr;
    _codecContext = nullptr;
    _swrContext = nullptr;
    _packet = nullptr;
    _frame = nullptr;

    _length = 0;
    _pos = 0;
    _audioIndex = -1;
    _remainSamples = 0;

    _cachedBuffer.clear();
}

void FFmpegDecoderPrivate::dispose() {
}

int FFmpegDecoderPrivate::orgBytesPerSample() {
    return _waveFormat.BitsPerSample() / 8;
}

qint64 FFmpegDecoderPrivate::src2dest_bytes(qint64 bytes) {
    return bytes / orgBytesPerSample() / _waveFormat.SampleRate() * _arguments.SampleRate *
           getArgsBytesPerSample(_arguments);
}

qint64 FFmpegDecoderPrivate::dest2src_bytes(qint64 bytes) {
    return (bytes / (float) getArgsBytesPerSample(_arguments) / _arguments.SampleRate *
            _waveFormat.SampleRate() * orgBytesPerSample());
}

int FFmpegDecoderPrivate::getArgsBytesPerSample(const WaveDecoder::WaveArguments &args) {
    return av_get_bytes_per_sample((AVSampleFormat) args.SampleFormat);
}
