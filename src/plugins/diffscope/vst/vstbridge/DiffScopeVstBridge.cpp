#include "DiffScopeVstBridge.h"
#include "QMSystem.h"
#include "rep_VstBridge_replica.h"

#define vstRep (ch->replica<VstBridgeReplica>())

namespace Vst {

    static const QString GLOBAL_UUID = "77F6E993-671E-4283-99BE-C1CD1FF5C09E";
    static int ipcTimeout = 5000; //TODO configurate in DiffScope settings

    bool DiffScopeVstBridge::checkSingleton() {
        QSystemSemaphore sema(GLOBAL_UUID, 1);
        bool isPrimary;
        sema.acquire();

        {
            //*nix shared memory clean-up
            QSharedMemory tempSharedMemory(GLOBAL_UUID + QString("guard"));
            tempSharedMemory.attach();
        }

        guardSharedMemory.reset(new QSharedMemory(GLOBAL_UUID + QString("guard")));
        isPrimary = guardSharedMemory->create(1);
        if(!isPrimary) {
            assert(!guardSharedMemory->isAttached());
        }
        sema.release();
        return isPrimary;
    }

    void DiffScopeVstBridge::releaseSingleton() {
        QSystemSemaphore sema(GLOBAL_UUID, 1);
        sema.acquire();
        guardSharedMemory->detach();
        sema.release();
    }

    void DiffScopeVstBridge::replicaInitialized() {
        auto reply = vstRep->initializeVst();
        if(reply.waitForFinished(ipcTimeout)) {
            callbacks->setStatus("Connected");
            isConnected = true;
        }
        if(!cachedState.isNull()) {
            stateChanged(cachedState);
            cachedState.clear();
        }
        if(cachedProcessInfo.sampleRate != 0) {
            initializeProcess(cachedProcessInfo.channelCount, cachedProcessInfo.maxBufferSize, cachedProcessInfo.sampleRate);
        }
    }

    void DiffScopeVstBridge::replicaNotInitialized() {
        callbacks->setStatus("Not Connected");
        isConnected = false;
    }

    bool DiffScopeVstBridge::initialize() {

        // check singleton
        if(!checkSingleton()) {
            callbacks->setError("Another DiffScope VSTi is already running.");
            return false;
        }

        // validate and read config file
        auto configPath = QMFs::appDataPath() + "/ChorusKit/DiffScope/vstconfig.txt";
        if(!QFileInfo::exists(configPath)) {
            callbacks->setError(QString("Cannot open config file: %1").arg(QDir::toNativeSeparators(configPath)).toUtf8());
            return false;
        }
        QFile configFile(configPath);
        configFile.open(QIODevice::ReadOnly);
        if(!configFile.isOpen()) {
            callbacks->setError(QString("Cannot open config file: %1").arg(QDir::toNativeSeparators(configPath)).toUtf8());
            return false;
        }
        configFile.readLine(); //vst bridge dir
        configFile.readLine(); //vst bridge file name
        QString path = configFile.readLine().trimmed();
        configFile.close();
        if(!QFileInfo(path).isExecutable()) {
            callbacks->setError(QString("Not a valid executable: %1").arg(path).toUtf8());
            return false;
        }

        // Connect to editor
        ch.reset(new CommunicationHelper);
        ch->start();
        alivePipe.listen(GLOBAL_UUID + "alive");
        ch->connectAsync<VstBridgeReplica>("local:" + GLOBAL_UUID, [=](bool isValid){
            isPending = false;
            if(isValid) {
                replicaInitialized();
            }
            QObject::connect(vstRep, &QRemoteObjectReplica::stateChanged, vstRep, [=](QRemoteObjectReplica::State state){
                if(state != QRemoteObjectReplica::Valid) {
                    replicaNotInitialized();
                } else {
                    replicaInitialized();
                }
            });
            QObject::connect(vstRep, &VstBridgeReplica::documentChanged, vstRep, [=]{
                callbacks->setDirty();
            });
        });

        // Start editor process
        editorProc.setProgram(path);
        editorProc.setArguments({"-vst"});
        if(!editorProc.startDetached()) {
            callbacks->setError("Cannot start DiffScope Editor");
            return false;
        } else {
            isPending = true;
        }
        return true;
    }

    void DiffScopeVstBridge::terminate() {
        ipcBuffer.detach();
        QObject::disconnect(vstRep, &VstBridgeReplica::documentChanged, vstRep, nullptr);
        releaseSingleton();
    }

    void DiffScopeVstBridge::bindEditorCallbacks(IEditorCallbacks *iEditorCallbacks) {
        callbacks = iEditorCallbacks;
    }

    void DiffScopeVstBridge::showWindow() {
        if(!isConnected && !isPending) {
            if(!editorProc.startDetached()) {
                callbacks->setError("Cannot start DiffScope Editor");
            } else {
                isPending = true;
            }
            return;
        }
        ch->invokeSync<void>([=]{
            vstRep->showWindow();
        });
    }

    void DiffScopeVstBridge::hideWindow() {
        if(!isConnected) return;
        ch->invokeSync<void>([=]{
            vstRep->hideWindow();
        });
    }

    // TODO

    bool DiffScopeVstBridge::initializeProcess(int channelCount, int maxBufferSize, double sampleRate) {
        cachedProcessInfo = {channelCount, maxBufferSize, sampleRate};
        if(!isConnected) {
            return true;
        }
        ipcBuffer.setKey(GLOBAL_UUID + "buffer");
        if(ipcBuffer.isAttached()) {
            ipcBuffer.detach();
        }
        if(ipcBuffer.attach()) {
            ipcBuffer.detach();
        }
        if(!ch->invokeSync<bool>([=](){
            auto reply = vstRep->initializeProcess(channelCount, maxBufferSize, sampleRate);
            if(!reply.waitForFinished(ipcTimeout)) return false;
            if(reply.returnValue()) {
                return ipcBuffer.attach();
            }
            return false;
        })) {
            callbacks->setError("Cannot initialize process.");
            return false;
        }
        return true;
    }
    bool DiffScopeVstBridge::processPlayback(bool isRealtime, bool isPlaying, int64_t position, int size, int channelCount, float *const *outputs) {
        if(!isConnected) return false;
        if(!ipcBuffer.isAttached()) return false;
        auto buf = reinterpret_cast<const float *>(ipcBuffer.constData());
        ch->invokeSync<void>([=]{
            vstRep->notifySwitchAudioBuffer(isRealtime, isPlaying, position, size, channelCount);
        });
        return ch->invokeSync<bool>([=]{
            QEventLoop eventLoop;
            QObject::connect(vstRep, &VstBridgeReplica::bufferSwitched, &eventLoop, &QEventLoop::exit);
            QTimer::singleShot(ipcTimeout, &eventLoop, [&]{
                eventLoop.exit(-1);
            });
            auto execRet = eventLoop.exec();
            if(execRet == 1) {
                for(int i = 0; i < channelCount; i++) {
                    memcpy(outputs[i], buf + i * size, size * sizeof(float));
                }
                return true;
            } else if(execRet == 0) {
                callbacks->setError("Playback processing failed.");
                return false;
            } else if(execRet == -1) {
                callbacks->setError("Playback processing timeout.");
                return false;
            }
            return false;
        });
    }
    void DiffScopeVstBridge::finalizeProcess() {
        ipcBuffer.detach();
        cachedProcessInfo = {};
        ch->invokeSync<void>([=]{
            vstRep->finalizeProcess();
        });
    }

    void DiffScopeVstBridge::stateChanged(const QByteArray &data) {
        cachedState = data;
        if(!isConnected) {
            return;
        }
        if(!ch->invokeSync<bool>([=]{
                auto reply = vstRep->openDataToEditor(data);
                if(!reply.waitForFinished(ipcTimeout)) return false;
                auto ret = reply.returnValue();
                return ret;
            })) {
            callbacks->setError("Cannot load file to editor.");
        }
    }

    void DiffScopeVstBridge::stateChanged(int size, const char *data) {
        stateChanged(QByteArray(data, size));
    }

    bool DiffScopeVstBridge::stateWillSave(int &size, const char *&data) {
        return ch->invokeSync<bool>([&]{
            auto reply = vstRep->saveDataFromEditor();
            if(!reply.waitForFinished(ipcTimeout)) return false;
            auto ret = reply.returnValue();
            if(ret[0] == (char)-128) return false;
            size = ret.size();
            auto dataCopy = new char[size];
            memcpy(dataCopy, ret.constData(), size);
            data = dataCopy;
            cachedState = ret;
            return true;
        });
    }

    void DiffScopeVstBridge::stateSaved(const char *data) {
        delete[] data;
    }
} // Vst