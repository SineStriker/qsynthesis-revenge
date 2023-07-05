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
            isConnected.store(true);
        }
        if(!cachedState.isNull()) {
            stateChanged(cachedState);
            cachedState.clear();
        }
    }

    void DiffScopeVstBridge::replicaNotInitialized() {
        callbacks->setStatus("Not Connected");
        isConnected.store(false);
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
        sbuf.detach();
        releaseSingleton();
    }

    void DiffScopeVstBridge::bindEditorCallbacks(IEditorCallbacks *iEditorCallbacks) {
        callbacks = iEditorCallbacks;
    }

    void DiffScopeVstBridge::showWindow() {
        if(!isConnected.load() && !isPending.load()) {
            if(!editorProc.startDetached()) {
                callbacks->setError("Cannot start DiffScope Editor");
            } else {
                isPending = true;
            }
            return;
        }
        ch->invokeSync<int>([=](){
            vstRep->showWindow();
            return 0;
        });
    }

    void DiffScopeVstBridge::hideWindow() {
        if(!isConnected.load()) return;
        ch->invokeSync<int>([=](){
            vstRep->hideWindow();
            return 0;
        });
    }

    // TODO

    bool DiffScopeVstBridge::initializeProcess(int channelCount, int maxBufferSize, double sampleRate) {
        return false;
    }
    bool DiffScopeVstBridge::processPlayback(bool isRealtime, bool isPlaying, int64_t position, int size, int channelCount,
                                             float *const *outputs) {
        return false;
    }
    void DiffScopeVstBridge::finalizeProcess() {
    }

    void DiffScopeVstBridge::stateChanged(const QByteArray &data) {
        if(!isConnected) {
            cachedState = data;
        } else {
            if(!ch->invokeSync<bool>([=](){
                    auto reply = vstRep->openDataToEditor(data);
                    if(!reply.waitForFinished(ipcTimeout)) return false;
                    auto ret = reply.returnValue();
                    return ret;
                })) {
                callbacks->setError("Cannot load file to editor.");
            }
        }
    }

    void DiffScopeVstBridge::stateChanged(int size, const char *data) {
        stateChanged(QByteArray(data, size));
    }

    bool DiffScopeVstBridge::stateWillSave(int &size, const char *&data) {
        return ch->invokeSync<bool>([&](){
            auto reply = vstRep->saveDataFromEditor();
            if(!reply.waitForFinished(ipcTimeout)) return false;
            auto ret = reply.returnValue();
            if(ret[0] == (char)-128) return false;
            size = ret.size();
            auto dataCopy = new char[size];
            memcpy(dataCopy, ret.constData(), size);
            data = dataCopy;
            return true;
        });
    }

    void DiffScopeVstBridge::stateSaved(const char *data) {
        delete[] data;
    }
} // Vst