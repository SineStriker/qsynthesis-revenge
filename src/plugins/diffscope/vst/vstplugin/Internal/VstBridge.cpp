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

namespace Vst::Internal {

    static VstBridge *m_instance = nullptr;

    VstBridge::VstBridge(QObject *parent): VstBridgeSource(parent), m_alivePipe(new QLocalSocket(this)) {
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
        if(m_clientAddOn) return false;
        m_alivePipe->setServerName(GLOBAL_UUID + "alive");
        if(!m_alivePipe->open()) {
            qWarning() << "Cannot connect to alive pipe.";
            return false;
        }
        openDataToEditor({});
        return true;
    }

    void VstBridge::finalizeVst() {
        qDebug() << "VstBridge: finalizeVst";
        if(m_clientAddOn) {
            auto iWin = m_clientAddOn->windowHandle()->cast<Core::IProjectWindow>();
            iWin->doc()->close();
        }
        m_clientAddOn = nullptr;
    }

    QByteArray VstBridge::saveDataFromEditor() {
        qDebug() << "VstBridge: saveDataFromEditor";
        static char failValue[1] = {-128};
        static const QByteArray failValueData(failValue, 1);
        if(!m_clientAddOn) return failValueData;
        QByteArray data;
        m_clientAddOn->windowHandle()->cast<Core::IProjectWindow>()->doc()->saveVST([&](const QByteArray &data_){
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
        if(m_clientAddOn) m_clientAddOn->windowHandle()->cast<Core::IProjectWindow>()->doc()->close();
        Core::IWindow::create<Core::IProjectWindow>(doc);
        auto rout = qobject_cast<Core::InitialRoutine *>(Core::ILoader::instance()->getFirstObject("choruskit_init_routine"));
        if(rout && m_isLoadFromInitialization) {
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
        if(m_clientAddOn) m_clientAddOn->showWindow();
    }

    void VstBridge::hideWindow() {
        qDebug() << "VstBridge: hideWindow";
        if(m_clientAddOn) m_clientAddOn->hideWindow();
    }
    bool VstBridge::initializeProcess(int channelCount, int maxBufferSize, double sampleRate) {
        return false;
    }
    void VstBridge::notifySwitchAudioBuffer(bool isRealtime, bool isPlaying, qint64 position, int bufferSize,
                                            int channelCount) {
    }
    void VstBridge::finalizeProcess() {
    }
    void VstBridge::handleRemoteCommand() const {
        if(m_clientAddOn) {
            Core::ICore::showHome();
        }
    }

} // Vst