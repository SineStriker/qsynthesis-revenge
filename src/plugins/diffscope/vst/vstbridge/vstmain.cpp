#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QSharedMemory>

#include <QMSystem.h>

#include "CommunicationHelper.h"
#include "ParameterTypes.h"
#include "VstHandle.h"
#include "rep_VstBridge_replica.h"

#define VST_EXPORT extern "C" Q_DECL_EXPORT
#define vstRep (h->ch->replica<VstBridgeReplica>())

using namespace Vst;

static const QString GLOBAL_UUID = "77F6E993-671E-4283-99BE-C1CD1FF5C09E";

static int ipcTimeout = 5000; //TODO configurate in DiffScope settings

static bool checkSingleton(VstHandle *h) {
    QSystemSemaphore sema(GLOBAL_UUID, 1);
    bool isPrimary;
    sema.acquire();

    {
        //*nix shared memory clean-up
        QSharedMemory tempSharedMemory(GLOBAL_UUID + QString("guard"));
        tempSharedMemory.attach();
    }

    h->guardSharedMemory.reset(new QSharedMemory(GLOBAL_UUID + QString("guard")));
    isPrimary = h->guardSharedMemory->create(1);
    if(!isPrimary) {
        assert(!h->guardSharedMemory->isAttached());
    }
    sema.release();
    return isPrimary;
}

static void releaseSingleton(VstHandle *h) {
    QSystemSemaphore sema(GLOBAL_UUID, 1);
    sema.acquire();
    h->guardSharedMemory->detach();
    sema.release();
}

VST_EXPORT VstHandle *HandleCreator() {
    return new VstHandle;
}

VST_EXPORT bool Initializer (VstHandle *h) {
    if(!checkSingleton(h)) {
        h->callbacks.setError(h->editorHelper, "Another DiffScope VSTi is already running.");
        return false;
    }
    auto configPath = QMFs::appDataPath() + "/ChorusKit/DiffScope/vstconfig.txt";
    if(!QFileInfo::exists(configPath)) {
        h->callbacks.setError(h->editorHelper, QString("Cannot open config file: %1").arg(QDir::toNativeSeparators(configPath)).toUtf8());
        return false;
    }
    QFile configFile(configPath);
    configFile.open(QIODevice::ReadOnly);
    if(!configFile.isOpen()) {
        h->callbacks.setError(h->editorHelper, QString("Cannot open config file: %1").arg(QDir::toNativeSeparators(configPath)).toUtf8());
        return false;
    }
    configFile.readLine(); //vst bridge dir
    configFile.readLine(); //vst bridge file name
    QString path = configFile.readLine().trimmed();
    configFile.close();
    if(!QFileInfo(path).isExecutable()) {
        h->callbacks.setError(h->editorHelper, QString("Not a valid executable: %1").arg(path).toUtf8());
        return false;
    }
    h->ch.reset(new CommunicationHelper);
    h->ch->start();
    h->alivePipe.listen(GLOBAL_UUID + "alive");
    h->ch->connectAsync<VstBridgeReplica>("local:" + GLOBAL_UUID, [=](bool isValid){
        h->isPending = false;
        if(isValid) {
            auto reply = vstRep->initializeVst();
            if(reply.waitForFinished(ipcTimeout)) {
                h->callbacks.setStatus(h->editorHelper, "Connected");
                h->isConnected.store(true);
            }
        }
        QObject::connect(vstRep, &QRemoteObjectReplica::stateChanged, vstRep, [=](QRemoteObjectReplica::State state){
            if(state != QRemoteObjectReplica::Valid) {
                h->callbacks.setStatus(h->editorHelper, "Not Connected");
                h->isConnected.store(false);
            } else {
                auto reply = vstRep->initializeVst();
                if(reply.waitForFinished(ipcTimeout)) {
                    h->callbacks.setStatus(h->editorHelper, "Connected");
                    h->isConnected.store(true);
                }
            }
        });
    });
    h->editorProc.setProgram(path);
    h->editorProc.setArguments({"-vst"});
    if(!h->editorProc.startDetached()) {
        h->callbacks.setError(h->editorHelper, "Cannot start DiffScope Editor");
        return false;
    } else {
        h->isPending = true;
    }
    return true;
}

VST_EXPORT void Terminator (VstHandle *h) {
    h->sbuf.detach();
    releaseSingleton(h);
}

VST_EXPORT void HandleDeleter (VstHandle *h) {
    delete h;
}

VST_EXPORT void WindowOpener (VstHandle *h) {
    if(!h->isConnected.load() && !h->isPending.load()) {
        if(!h->editorProc.startDetached()) {
            h->callbacks.setError(h->editorHelper, "Cannot start DiffScope Editor");
        } else {
            h->isPending = true;
        }
        return;
    }
    h->ch->invokeSync<int>([=](){
        vstRep->showWindow();
        return 0;
    });
}

VST_EXPORT void WindowCloser (VstHandle *h) {
    if(!h->isConnected.load()) return;
    h->ch->invokeSync<int>([=](){
        vstRep->hideWindow();
        return 0;
    });
}

VST_EXPORT bool PlaybackProcessor (VstHandle *h, const PlaybackParameters &parameters, float *const *const * outputs) {
    if(!h->isConnected.load()) return false;
    return h->ch->invokeSync<bool>([&](){
//        auto ret = h->ch.replica<VstBridgeReplica>()->testProcess(parameters.projectTimeSamples, parameters.numSamples);
//        if(!ret.waitForFinished(IPC_TIMEOUT)) return false;
//        if(!ret.returnValue()) return false;
//        h->sbuf.lock();
//        memcpy(outputs[0][0], h->sbuf.constData(), parameters.numSamples * sizeof(float));
//        memcpy(outputs[0][1], h->sbuf.constData(), parameters.numSamples * sizeof(float));
//        h->sbuf.unlock();
        return false;
    });
}

VST_EXPORT void StateChangedCallback (VstHandle *h, uint64_t size, const char *data) {
    QByteArray sentData(data, size);
    if(!h->ch->invokeSync<bool>([=](){
        auto reply = vstRep->openDataToEditor(sentData);
        if(!reply.waitForFinished(ipcTimeout)) return false;
        auto ret = reply.returnValue();
        return ret;
    })) {
        h->callbacks.setError(h->editorHelper, "Cannot load file to editor.");
    }
}

VST_EXPORT bool StateWillSaveCallback (VstHandle *h, uint64_t &size, const uint8_t *&data) {
    return h->ch->invokeSync<bool>([&](){
        auto reply = vstRep->saveDataFromEditor();
        if(!reply.waitForFinished(ipcTimeout)) return false;
        auto ret = reply.returnValue();
        if(ret[0] == (char)-128) return false;
        size = ret.size();
        auto dataCopy = new uint8_t[size];
        memcpy(dataCopy, ret.constData(), size);
        data = dataCopy;
        return true;
    });
}

VST_EXPORT void StateSavedAsyncCallback (VstHandle *h, const uint8_t * dataToFree) {
    delete[] dataToFree;
}

VST_EXPORT void CallbacksBinder (VstHandle *h, void *eh, const Callbacks &callbacks_) {
    h->editorHelper = eh;
    h->callbacks = callbacks_;
}

VST_EXPORT bool ProcessInitializer(VstHandle *h, int32_t totalNumOutputChannels, int32_t maxBufferSize, double sampleRate) {
//    h->sbuf.setKey("77F6E993-671E-4283-99BE-C1CD1FF5C09E");
//    h->sbuf.create(maxBufferSize * sizeof(float));
    return true;
}

VST_EXPORT void ProcessFinalizer(VstHandle *h) {

}