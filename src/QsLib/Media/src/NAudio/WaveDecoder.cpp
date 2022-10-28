#include "WaveDecoder.h"

WaveDecoder::~WaveDecoder() {
}

WaveDecoder::WaveArguments WaveDecoder::arguments() const {
    return _arguments;
}

void WaveDecoder::setArguments(const WaveArguments &arguments) {
    _arguments = arguments;
}
