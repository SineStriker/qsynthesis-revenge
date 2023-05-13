#include <QDataStream>
#include <QLocalServer>
#include <QLocalSocket>
#include <QSharedMemory>

#include "DiffScopeEditor.h"
#include "IpcConnect.h"

#define VST_EXPORT extern "C" Q_DECL_EXPORT
using namespace Vst;

VST_EXPORT bool SingletonChecker() {
    return true;
}

DiffScopeEditor *diffScopeEditor;

VST_EXPORT bool Initializer() {
    diffScopeEditor = new DiffScopeEditor;
    return diffScopeEditor->start();
}

VST_EXPORT bool Terminator() {
    diffScopeEditor->stop();
    diffScopeEditor->deleteLater();
    diffScopeEditor = nullptr;
    return true;
}

const quint8 OPEN_WINDOW_SIGNAL = 0x01;
const quint8 CLOSE_WINDOW_SIGNAL = 0x02;
const quint8 PROCESS_SIGNAL = 0x03;
const quint8 STATE_CHANGED_SIGNAL = 0x04;
const quint8 STATE_SAVE_SIGNAL = 0x05;
const quint8 SET_DIRTY_SIGNAL = 0x06;
const quint8 PROCESS_INITIALIZE_SIGNAL = 0x07;

VST_EXPORT void WindowOpener() {
    if(diffScopeEditor->status() == DiffScopeEditor::Disconnected) {
        diffScopeEditor->start();
    } else {
        diffScopeEditor->mainChannel()->send(OPEN_WINDOW_SIGNAL);
    }
}

VST_EXPORT void WindowCloser() {
    if(diffScopeEditor->status() == DiffScopeEditor::Disconnected) {
        diffScopeEditor->start();
    } else {
        diffScopeEditor->mainChannel()->send(CLOSE_WINDOW_SIGNAL);
    }

}

struct PlaybackParameters {
    double sampleRate;
    qint64 position;
    qint32 bufferSize;
};

VST_EXPORT bool PlaybackProcessor(const PlaybackParameters *playbackParameters, bool isPlaying, int32_t numOutputs, float *const *const *outputs) {
    QByteArray args;
    QDataStream out(&args, QIODevice::WriteOnly);
    out << playbackParameters->sampleRate << playbackParameters->position << playbackParameters->bufferSize << isPlaying << numOutputs;
    auto ret = diffScopeEditor->processingChannel()->send(PROCESS_SIGNAL, args);
    if(ret.size() < 1) return false;
    QDataStream in(ret);
    bool success;
    in >> success;
    if(!success) return false;
    diffScopeEditor->processingServer()->lockSharedMemory();
    for(int i = 0; i < numOutputs; i++) {
        for(int j = 0; j < 2; j++) {
            memcpy(
                outputs[i][j],
                (float *)diffScopeEditor->processingServer()->sharedData() + (i * 2 + j) * playbackParameters->bufferSize,
                playbackParameters->bufferSize * sizeof(float)
            );
        }
    }
    diffScopeEditor->processingServer()->unlockSharedMemory();
    return true;
}

VST_EXPORT bool StateChangedCallback(qint64 size, const char *data) {
    QByteArray args;
    QDataStream out(&args, QIODevice::WriteOnly);
    out << QByteArray(data, size);
    auto ret = diffScopeEditor->mainChannel()->send(STATE_CHANGED_SIGNAL, args);
    if(ret.size() < 1) return false;
    QDataStream in(ret);
    bool success;
    in >> success;
    return success;
}

VST_EXPORT bool StateWillSaveCallback(qint64 &size, const char *&data) {
    auto ret = diffScopeEditor->mainChannel()->send(STATE_SAVE_SIGNAL);
    if(ret.size() < sizeof(bool) + sizeof(quint64)) return false;
    QDataStream in(ret);
    bool success;
    in >> success;
    if(!success) return false;
    QByteArray content;
    in >> content;
    size = content.size();
    data = content.constData();
    return true;
}

VST_EXPORT void StateSavedAsyncCallback(const char* dataToFree) {

}

VST_EXPORT void DirtySetterBinder(void (*setDirty)(bool)) {
    QObject::connect(diffScopeEditor->dirtySettingChannel(), &IpcReceiveChannel::received, [=](quint8 signal, const QByteArray &data, QByteArray &ret){
        if(signal != SET_DIRTY_SIGNAL) return;
        QDataStream in(data);
        bool isDirty;
        in >> isDirty;
        setDirty(isDirty);
    });
}

VST_EXPORT bool ProcessInitializer(bool isOffline, qint32 maxBufferSize, double sampleRate) { //TODO max buffer size
    diffScopeEditor->mainChannel()->allocate(maxBufferSize * 2 * 16);
    QByteArray args;
    QDataStream out(&args, QIODevice::WriteOnly);
    out << isOffline << maxBufferSize << sampleRate;
    auto ret = diffScopeEditor->mainChannel()->send(PROCESS_INITIALIZE_SIGNAL, args);
    if(ret.size() < 1) return false;
    QDataStream in(ret);
    bool success;
    in >> success;
    return success;
}