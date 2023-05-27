#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QSharedMemory>

#include <QMSystem.h>

#include "CommunicationHelper.h"
#include "ParameterTypes.h"
#include "rep_VstBridge_replica.h"

#define VST_EXPORT extern "C" Q_DECL_EXPORT

using namespace Vst;

static Callbacks callbacks;

static CommunicationHelper ch;

static QProcess editorProc;

static std::atomic<bool> isConnected = false;

static QSharedMemory sbuf;

static char GLOBAL_UUID[] = "77F6E993-671E-4283-99BE-C1CD1FF5C09E";

static const int IPC_TIMEOUT = 1000; //TODO configurate in DiffScope settings

static bool checkSingleton() {
    QSystemSemaphore sema(GLOBAL_UUID, 1);
    bool isPrimary;
    sema.acquire();

    {
        //*nix shared memory clean-up
        QSharedMemory tempSharedMemory(GLOBAL_UUID + QString("guard"));
        tempSharedMemory.attach();
    }

    static QSharedMemory guardSharedMemory(GLOBAL_UUID + QString("guard"));
    if (guardSharedMemory.attach()) {
        isPrimary = false;
    }
    else {
        guardSharedMemory.create(1);
        isPrimary = true;
    }
    sema.release();
    return isPrimary;
}

VST_EXPORT bool Initializer () {
    if(!checkSingleton()) {
        callbacks.setError("Another DiffScope VSTi is already running.");
        return false;
    }
    auto configPath = QMFs::appDataPath() + "/ChorusKit/DiffScope/vstconfig.txt";
    if(!QFileInfo::exists(configPath)) {
        callbacks.setError(QString("Cannot open config file: %1").arg(QDir::toNativeSeparators(configPath)).toUtf8());
        return false;
    }
    QFile configFile(configPath);
    configFile.open(QIODevice::ReadOnly);
    if(!configFile.isOpen()) {
        callbacks.setError(QString("Cannot open config file: %1").arg(QDir::toNativeSeparators(configPath)).toUtf8());
        return false;
    }
    configFile.readLine(); //vst bridge dir
    configFile.readLine(); //vst bridge file name
    QString path = configFile.readLine().trimmed();
    configFile.close();
    if(!QFileInfo(path).isExecutable()) {
        callbacks.setError(QString("Not a valid executable: %1").arg(path).toUtf8());
        return false;
    }
    ch.start();
    ch.connectAsync<VstBridgeReplica>("local:77F6E993-671E-4283-99BE-C1CD1FF5C09E", [](bool isValid){
        if(isValid) {
            callbacks.setStatus("Connected");
            isConnected.store(true);
        }
        QObject::connect(ch.replica<VstBridgeReplica>(), &QRemoteObjectReplica::stateChanged, ch.replica<VstBridgeReplica>(), [](QRemoteObjectReplica::State state){
            if(state != QRemoteObjectReplica::Valid) {
                callbacks.setStatus("Not Connected");
                isConnected.store(false);
            }
        });
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
    ch.stop();
    sbuf.detach();
}

VST_EXPORT void WindowOpener () {

}

VST_EXPORT void WindowCloser () {

}

VST_EXPORT bool PlaybackProcessor (const PlaybackParameters &parameters, float *const *const * outputs) {
    if(!isConnected.load()) return false;
    return ch.invokeSync<bool>([&](){
        auto ret = ch.replica<VstBridgeReplica>()->testProcess(parameters.projectTimeSamples, parameters.numSamples);
        if(!ret.waitForFinished(IPC_TIMEOUT)) return false;
        if(!ret.returnValue()) return false;
        sbuf.lock();
        memcpy(outputs[0][0], sbuf.constData(), parameters.numSamples * sizeof(float));
        memcpy(outputs[0][1], sbuf.constData(), parameters.numSamples * sizeof(float));
        sbuf.unlock();
        return true;
    });
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
    sbuf.setKey("77F6E993-671E-4283-99BE-C1CD1FF5C09E");
    sbuf.create(maxBufferSize * sizeof(float));
    return true;
}
