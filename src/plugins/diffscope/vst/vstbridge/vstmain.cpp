#include <QFile>
#include <QFileInfo>
#include <QMSystem.h>
#include <QProcess>
#include <iostream>

#include "CommunicationHelper.h"
#include "ParameterTypes.h"
#include "rep_SimpleSwitch_replica.h"
#include "rep_VstBridge_replica.h"

#define VST_EXPORT extern "C" Q_DECL_EXPORT

using namespace Vst;

static Callbacks callbacks;

static CommunicationHelper ch;

static QProcess editorProc;

static std::atomic<bool> isConnected = false;

VST_EXPORT bool Initializer () {
    QFile configFile(QMFs::appDataPath() + "/ChorusKit/DiffScope/vstconfig.txt");
    configFile.open(QIODevice::ReadOnly);
    configFile.readLine();
    configFile.readLine();
    QString path = configFile.readLine();
    if(!QFileInfo(path).isExecutable()) {
        callbacks.setError(QString("Not a valid executable: %1").arg(path).toUtf8());
        return false;
    }
    ch.start();
    ch.connectAsync<VstBridgeReplica>("local:77F6E993-671E-4283-99BE-C1CD1FF5CO9E", 30000, [](bool isValid){
        if(isValid) {
            callbacks.setStatus("Connected");
            isConnected.store(true);
        }
    });
    editorProc.setProgram(path);
    editorProc.setArguments({"-vst"});
    if(!editorProc.startDetached()) {
        callbacks.setError("Cannot start DiffScope Editor");
        return false;
    }
    return true;
}

VST_EXPORT void Terminator () {

}

VST_EXPORT void WindowOpener () {

}

VST_EXPORT void WindowCloser () {

}

VST_EXPORT bool PlaybackProcessor (const PlaybackParameters &parameters, float *const *const * outputs) {
    return false;
}

VST_EXPORT void StateChangedCallback (uint64_t size, const uint8_t *data) {

}

VST_EXPORT bool StateWillSaveCallback (uint64_t &size, const uint8_t *&data) {
    return false;
}

VST_EXPORT void StateSavedAsyncCallback (const uint8_t * dataToFree) {

}

VST_EXPORT void CallbacksBinder (const Callbacks &callbacks_) {
    callbacks = callbacks_;
}

VST_EXPORT bool ProcessInitializer(int32_t totalNumOutputChannels, int32_t maxBufferSize, double sampleRate) {
    return false;
}
