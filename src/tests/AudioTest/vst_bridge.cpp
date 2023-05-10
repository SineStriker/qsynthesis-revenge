//
// Created by Crs_1 on 2023/5/10.
//

#include "AudioTrack.h"
#include "Pcm32BitFloatAudioSource.h"
#include "VstAudioOutputManager.h"

extern "C" Q_DECL_EXPORT bool SingletonChecker() {
    return true;
}

AudioTrack *track;
Pcm32BitFloatAudioSource *src1;
Pcm32BitFloatAudioSource *src2;
VstAudioOutputManager *vstMgr;


extern "C" Q_DECL_EXPORT bool Initializer() {
    track = new AudioTrack(2, 48000);
    src1 = new Pcm32BitFloatAudioSource("D:\\Downloads\\test_source1.pcm", 2, 48000);
    src2 = new Pcm32BitFloatAudioSource("D:\\Downloads\\test_source2.pcm", 2, 48000);
    track->addSource(src1);
    track->addSource(src2);
    if(!track->open()) return false;
    vstMgr = new VstAudioOutputManager;
    vstMgr->output(0)->addSource(track);
    return true;
}

extern "C" Q_DECL_EXPORT bool Terminator() {
    delete track;
    delete src1;
    delete src2;
    delete vstMgr;
    return true;
}

extern "C" Q_DECL_EXPORT void WindowOpener() {

}

extern "C" Q_DECL_EXPORT void WindowCloser() {

}

extern "C" Q_DECL_EXPORT bool PlaybackProcessor(const VstAudioOutputManager::PlaybackParameters *playbackParameters, bool isPlaying, int32_t numOutputs, float *const *const *outputs) {
    return vstMgr->vstProcess(playbackParameters, isPlaying, numOutputs, outputs);
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

extern "C" Q_DECL_EXPORT bool ProcessInitializer(bool isOffline, double sampleRate) {
    return vstMgr->vstProcessInitializer(isOffline, sampleRate);
}