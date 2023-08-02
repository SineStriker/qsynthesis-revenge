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

namespace Vst::Internal {

    static VstBridge *m_instance = nullptr;

    VstBridge::VstBridge(QObject *parent): VstBridgeSource(parent), m_alivePipe(new QLocalSocket(this)), m_ipcBuffer(new QSharedMemory(VstHelper::globalUuid() + "buffer", this)) {
        m_instance = this;
        connect(m_alivePipe, &QLocalSocket::disconnected, this, &VstBridge::finalizeVst);
    }

    VstBridge::~VstBridge() {
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
        if(m_ipcBuffer->isAttached()) {
            m_ipcBuffer->detach();
        }
        if(m_ipcBuffer->attach()) {
            m_ipcBuffer->detach();
        }
        if(!m_ipcBuffer->create(maxBufferSize * channelCount * sizeof(float) * 2)) {
            return false;
        }
        VstHelper::instance()->connectionStatus.isProcessing = true;
        VstHelper::instance()->connectionStatus.channelCount = channelCount;
        VstHelper::instance()->connectionStatus.bufferSize = maxBufferSize;
        VstHelper::instance()->connectionStatus.sampleRate = sampleRate;
        return true;
    }
    void VstBridge::notifySwitchAudioBuffer(bool isRealtime, bool isPlaying, qint64 position, int bufferSize, int channelCount) {
        //TODO
        emit bufferSwitched(true);
    }
    void VstBridge::finalizeProcess() {
        m_ipcBuffer->detach();
        VstHelper::instance()->connectionStatus.isProcessing = false;
    }

} // Vst