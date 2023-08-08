//
// Created by Crs_1 on 2023/5/26.
//

#include "VstBridge.h"

#include <QSharedMemory>
#include <QLocalSocket>
#include <QTimer>
#include <QThread>

#include <Window/IProjectWindow.h>
#include <Window/IHomeWindow.h>
#include "ICore.h"
#include <CoreApi/ILoader.h>

#include "AddOn/VstClientAddOn.h"
#include "VstHelper.h"
#include "Connection/VstPlaybackWorker.h"

namespace Vst::Internal {

    static VstBridge *m_instance = nullptr;

    VstBridge::VstBridge(QObject *parent):
          VstBridgeSource(parent),
          m_alivePipe(new QLocalSocket(this)),
          m_processBufferSharedMemory(new QSharedMemory(VstHelper::globalUuid() + "buffer", this)),
          m_processDataSharedMemory(new QSharedMemory(VstHelper::globalUuid() + "process_data", this)),
          m_vstPlaybackWorkerThread(new QThread(this)),
          m_worker(new VstPlaybackWorker(m_processDataSharedMemory, m_processBufferSharedMemory))
    {
        m_instance = this;
        m_worker->moveToThread(m_vstPlaybackWorkerThread);
        connect(m_alivePipe, &QLocalSocket::disconnected, this, &VstBridge::finalizeVst);
    }

    VstBridge::~VstBridge() {
        VstBridge::finalizeVst();
        delete m_worker;
        m_instance = nullptr;
    }

    VstBridge *VstBridge::instance() {
        return m_instance;
    }

    bool VstBridge::initializeVst() {
        qDebug() << "VstBridge: initializeVst";
        if(VstHelper::instance()->addOn) return false;
        m_alivePipe->setServerName(VstHelper::globalUuid() + "alive");
        if(!m_alivePipe->open()) {
            qWarning() << "Cannot connect to alive pipe.";
            return false;
        }
        VstHelper::instance()->connectionStatus.isConnected = true;
        if(m_processDataSharedMemory->isAttached()) {
            m_processDataSharedMemory->detach();
        }
        if(!m_processDataSharedMemory->attach()) {
            return false;
        }
        connect(m_vstPlaybackWorkerThread, &QThread::started, m_worker, &VstPlaybackWorker::start);
        qDebug() << "Main thread:" << QThread::currentThreadId();
        m_vstPlaybackWorkerThread->start();
        openDataToEditor({});
        return true;
    }

    void VstBridge::finalizeVst() {
        qDebug() << "VstBridge: finalizeVst";
        finalizeProcess();
        if(VstHelper::instance()->addOn) {
            auto iWin = VstHelper::instance()->addOn->windowHandle()->cast<Core::IProjectWindow>();
            iWin->doc()->close();
        }
        m_worker->quit();
        m_vstPlaybackWorkerThread->quit();
        m_vstPlaybackWorkerThread->wait();
        m_processDataSharedMemory->detach();
        VstHelper::instance()->connectionStatus.isConnected = false;
        VstHelper::instance()->addOn = nullptr;
    }

    QByteArray VstBridge::saveDataFromEditor() {
        qDebug() << "VstBridge: saveDataFromEditor";
        static char failValue[1] = {-128};
        static const QByteArray failValueData(failValue, 1);
        if(!VstHelper::instance()->addOn) return failValueData;
        QByteArray data;
        VstHelper::instance()->addOn->windowHandle()->cast<Core::IProjectWindow>()->doc()->saveVST([&](const QByteArray &data_){
            data = data_;
            return true;
        });
        return data;
    }

    bool VstBridge::openDataToEditor(const QByteArray &data) {
        qDebug() << "VstBridge: openDataToEditor";
        auto doc = new Core::DspxDocument(Core::DspxDocument::VSTMode);
        auto pseudoName = tr("VST Mode");
        if(data.isEmpty()) {
            doc->makeNew(pseudoName);
        } else {
            if (!doc->openRawData(pseudoName, data)) {
                doc->makeNew(pseudoName);
            }
        }
        if(VstHelper::instance()->addOn) VstHelper::instance()->addOn->windowHandle()->cast<Core::IProjectWindow>()->doc()->close();
        Core::IWindow::create<Core::IProjectWindow>(doc);
        auto rout = qobject_cast<Core::InitialRoutine *>(Core::ILoader::instance()->getFirstObject("choruskit_init_routine"));
        if(rout && VstHelper::instance()->isLoadFromInitialization) {
            connect(rout, &Core::InitialRoutine::done, this, [=]{
                if(Core::IHomeWindow::instance()) {
                    Core::IHomeWindow::instance()->window()->close();
                }
            });
        }
        connect(doc, &Core::IDocument::changed, this, [=]{
            emit this->documentChanged();
        });
        return true;
    }

    void VstBridge::showWindow() {
        qDebug() << "VstBridge: showWindow";
        if(VstHelper::instance()->addOn) VstHelper::instance()->addOn->showWindow();
    }

    void VstBridge::hideWindow() {
        qDebug() << "VstBridge: hideWindow";
        if(VstHelper::instance()->addOn) VstHelper::instance()->addOn->hideWindow();
    }
    bool VstBridge::initializeProcess(int channelCount, int maxBufferSize, double sampleRate) {
        return false;
    }
    void VstBridge::notifySwitchAudioBuffer(bool isRealtime, bool isPlaying, qint64 position, int bufferSize, int channelCount) {

    }
    void VstBridge::finalizeProcess() {

    }

} // Vst