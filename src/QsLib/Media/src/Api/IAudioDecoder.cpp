#include "IAudioDecoder.h"

IAudioDecoder::IAudioDecoder(const WaveArguments &args, QObject *parent)
    : QObject(parent), WaveDecoder(args) {
}

IAudioDecoder::~IAudioDecoder() {
}
