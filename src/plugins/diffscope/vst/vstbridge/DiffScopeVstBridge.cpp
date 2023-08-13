#include "DiffScopeVstBridge.h"
#include <QMSystem.h>
#include <rep_VstBridge_replica.h>
#include <VstProcessData.h>

#define vstRep (ch->replica<VstBridgeReplica>())

namespace Vst {

    static QString GLOBAL_UUID = "77F6E993-671E-4283-99BE-C1CD1FF5C09E";
    static int ipcTimeout = 5000;

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
        if(!processCallMutex->tryAcquire(ipcTimeout)) {
            return;
        }
        auto reply = vstRep->initializeVst();
        if(reply.waitForFinished(ipcTimeout)) {
            callbacks->setStatus("Connected");
            callbacks->setError("");
        } else {
            processCallMutex->release();
            return;
        }
        isConnected = true;
        if(!cachedState.isNull()) {
            stateChanged(cachedState);
        }
        if(cachedProcessInfo.sampleRate != 0) {
            initializeProcess(cachedProcessInfo.channelCount, cachedProcessInfo.maxBufferSize, cachedProcessInfo.sampleRate);
        }
    }

    void DiffScopeVstBridge::replicaNotInitialized() {
        isConnected = false;
        isPending = false;
        callbacks->setStatus("Not Connected");
        callbacks->setError("DiffScope Editor exited abnormally.");

        QMutexLocker locker(&processSyncMutex);

        terminate();
        initialize(false);
    }

    bool DiffScopeVstBridge::initialize() {
        return initialize(true);
    }

    bool DiffScopeVstBridge::initialize(bool isFirstInitialization) {

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
        if(!QFileInfo(path).isExecutable()) {
            callbacks->setError(QString("Not a valid executable: %1").arg(path).toUtf8());
            return false;
        }
        QString ipcTimeoutStr = configFile.readLine().trimmed();
        {
            bool ok = false;
            ipcTimeoutStr.toInt(&ok);
            if(ok) ipcTimeout = ipcTimeoutStr.toInt();
        }
        QString mainKey = configFile.readLine().trimmed();
        if(!mainKey.isEmpty()) {
            GLOBAL_UUID = mainKey;
        }


        configFile.close();

        editorProc.setProgram(path);
        editorProc.setArguments({"-vst"});

        // Connect to editor
        // TODO: handle error
        if(isFirstInitialization) {
            ch.reset(new CommunicationHelper);
            ch->start();
            alivePipe.listen(GLOBAL_UUID + "alive");
        }

        processDataSharedMemory.setKey(GLOBAL_UUID + "process_data");

        if(processDataSharedMemory.isAttached()) {
            processDataSharedMemory.detach();
        }
        if(processDataSharedMemory.attach()) {
            processDataSharedMemory.detach();
        }
        processDataSharedMemory.create(sizeof(VstProcessData));
        memset(processDataSharedMemory.data(), 0, sizeof(VstProcessData));
        if(isFirstInitialization) processCallMutex.reset(new QSystemSemaphoreExtended(GLOBAL_UUID + "process_call", 1, QSystemSemaphore::Create));
        ch->connectAsync<VstBridgeReplica>("local:" + GLOBAL_UUID, [=](bool isValid){
            isPending = false;
            if(isValid) {
                replicaInitialized();
            }
            QObject::connect(vstRep, &QRemoteObjectReplica::stateChanged, vstRep, [=](QRemoteObjectReplica::State state){
                if(state != QRemoteObjectReplica::Valid) {
                    replicaNotInitialized();
                }
            });
            QObject::connect(vstRep, &VstBridgeReplica::documentChanged, vstRep, [=]{
                callbacks->setDirty();
            });
        });
        if(isFirstInitialization) {
            startEditorProcess();
        }
        return true;
    }

    bool DiffScopeVstBridge::startEditorProcess() {
        if(!editorProc.startDetached()) {
            callbacks->setError("Cannot start DiffScope Editor");
            return false;
        } else {
            isPending = true;
        }
        return true;
    }

    void DiffScopeVstBridge::terminate() {
        processBufferSharedMemory.detach();
        processDataSharedMemory.detach();
        processCallMutex->release();
        QObject::disconnect(vstRep, &VstBridgeReplica::documentChanged, vstRep, nullptr);
        releaseSingleton();
    }

    void DiffScopeVstBridge::bindEditorCallbacks(IEditorCallbacks *iEditorCallbacks) {
        callbacks = iEditorCallbacks;
    }

    void DiffScopeVstBridge::showWindow() {
        if(!isConnected && !isPending) {
            startEditorProcess();
        } else if(isConnected) {
            ch->invokeSync<int>([=]{
                vstRep->showWindow();
                return 0;
            });
        }
    }

    void DiffScopeVstBridge::hideWindow() {
        if(!isConnected) return;
        ch->invokeSync<int>([=]{
            vstRep->hideWindow();
            return 0;
        });
    }

    // TODO

    bool DiffScopeVstBridge::initializeProcess(int channelCount, int maxBufferSize, double sampleRate) {
        cachedProcessInfo = {channelCount, maxBufferSize, sampleRate};
        if(!isConnected) {
            return true;
        }
        processBufferSharedMemory.setKey(GLOBAL_UUID + "buffer");
        if(processBufferSharedMemory.isAttached()) {
            processBufferSharedMemory.detach();
        }
        if(processBufferSharedMemory.attach()) {
            processBufferSharedMemory.detach();
        }
        processBufferSharedMemory.create(std::max(sizeof(VstBufferSwitchData), channelCount * maxBufferSize * sizeof(float) * 2));

        auto buf = reinterpret_cast<const float *>(processBufferSharedMemory.constData());
        planarOutputData.resize(channelCount);
        for(int i = 0; i < channelCount; i++) {
            planarOutputData[i] = buf + i * (maxBufferSize * 2);
        }
        bufferSwitchData = reinterpret_cast<VstBufferSwitchData *>(processBufferSharedMemory.data());
        processData = reinterpret_cast<VstProcessData *>(processDataSharedMemory.data());

        processData->channelCount = channelCount;
        processData->bufferSize = maxBufferSize;
        processData->sampleRate = sampleRate;
        processData->flag = VstProcessData::ProcessInitializationRequest;

        QMutexLocker locker(&processSyncMutex);

        processCallMutex->release();
        while(isConnected) {
            if(!processCallMutex->tryAcquire(ipcTimeout)) {
                return false;
            }
            if(processData->flag == VstProcessData::ProcessInitializationResponse) {
                return true;
            } else if(processData->flag == VstProcessData::ProcessInitializationError) {
                return false;
            }
            processCallMutex->release();
        }
        return false;
    }
    bool DiffScopeVstBridge::processPlayback(bool isRealtime, bool isPlaying, int64_t position, int size, int channelCount, float *const *outputs) {
        if(!isConnected) return false;
        if(!processBufferSharedMemory.isAttached()) return false;
        if(!processData) return false;
        bufferSwitchData->isRealtime = isRealtime;
        bufferSwitchData->isPlaying = isPlaying;
        bufferSwitchData->position = position;
        bufferSwitchData->size = size;
        bufferSwitchData->channelCount = channelCount;
        processData->flag = VstProcessData::BufferSwitchRequest;

        QMutexLocker locker(&processSyncMutex);

        processCallMutex->release();
        while(isConnected) {
            if(!processCallMutex->tryAcquire(ipcTimeout)) {
                return false;
            }
            if(processData->flag == VstProcessData::BufferSwitchFinished) {
                for(int i = 0; i < channelCount; i++) {
                    memcpy(outputs[i], planarOutputData[i], size * sizeof(float));
                }
                return true;
            } else if(processData->flag == VstProcessData::BufferSwitchError) {
                return false;
            }
            processCallMutex->release();
        }
        return false;
    }
    void DiffScopeVstBridge::finalizeProcess() {
        processBufferSharedMemory.detach();
        cachedProcessInfo = {};
        if(processData) {
            processData->sampleRate = 0;
            processData->flag = VstProcessData::ProcessInitializationRequest;

            QMutexLocker locker(&processSyncMutex);

            processCallMutex->release();
            while(isConnected) {
                if(!processCallMutex->tryAcquire(ipcTimeout)) {
                    break;
                }
                if(processData->flag == VstProcessData::ProcessInitializationResponse) {
                    break;
                }
                processCallMutex->release();
            }
        }
        processData = nullptr;
        bufferSwitchData = nullptr;
        planarOutputData.resize(0);
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
        if(!isConnected) return false;
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