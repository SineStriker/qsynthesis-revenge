//
// Created by Crs_1 on 2023/5/10.
//

#include "AudioTrack.h"
#include "Pcm32BitFloatAudioSource.h"
struct PlaybackParameters {
    double sampleRate;
    qint64 position;
    qint32 bufferSize;
};

extern "C" Q_DECL_EXPORT bool SingletonChecker() {
    return true;
}

AudioTrack *track;
Pcm32BitFloatAudioSource *src1;
Pcm32BitFloatAudioSource *src2;


extern "C" Q_DECL_EXPORT bool Initializer() {
    track = new AudioTrack(2, 48000);
    src1 = new Pcm32BitFloatAudioSource("D:\\Downloads\\test_source1.pcm", 2, 48000);
    src2 = new Pcm32BitFloatAudioSource("D:\\Downloads\\test_source2.pcm", 2, 48000);
    track->addSource(src1);
    track->addSource(src2);
    return track->open();
}

extern "C" Q_DECL_EXPORT bool Terminator() {
    delete track;
    delete src1;
    delete src2;
    return true;
}

extern "C" Q_DECL_EXPORT void WindowOpener() {

}

extern "C" Q_DECL_EXPORT void WindowCloser() {

}

extern "C" Q_DECL_EXPORT bool PlaybackProcessor(const PlaybackParameters *playbackParameters, bool isPlaying, int32_t numOutputs, float *const *const *outputs) {
    AudioBufferList buf(2, playbackParameters->bufferSize);
    auto ret = track->read(buf, playbackParameters->bufferSize);
    if(ret == playbackParameters->bufferSize) {
        ::memcpy(outputs[0][0], buf[0].data(), playbackParameters->bufferSize * sizeof(float));
        ::memcpy(outputs[0][1], buf[1].data(), playbackParameters->bufferSize * sizeof(float));
        return true;
    } else {
        track->setPos(0);
        return false;
    }
}

extern "C" Q_DECL_EXPORT bool StateChangedCallback(qint64 size, const char *data) {
    return true;
}

extern "C" Q_DECL_EXPORT bool StateWillSaveCallback(qint64 &size, char *&data) {
    size = 0;
    return true;
}

extern "C" Q_DECL_EXPORT void StateSavedAsyncCallback(uint8_t* dataToFree) {
    return;
}

extern "C" Q_DECL_EXPORT void DirtySetterBinder(void (*setDirty)(bool)) {
    return;
}