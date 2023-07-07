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

#include "AddOn/VstClientAddOn.h"

namespace Vst::Internal {

    static VstBridge *m_instance = nullptr;

    VstBridge::VstBridge(QObject *parent): VstBridgeSource(parent), m_alivePipe(new QLocalSocket(this)) {
        m_instance = this;
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
        connect(m_alivePipe, &QLocalSocket::disconnected, this, &VstBridge::finalizeVst);
        openDataToEditor({});
        return true;
    }

    void VstBridge::finalizeVst() {
        qDebug() << "VstBridge: finalizeVst";
        if(m_clientAddOn) emit m_clientAddOn->windowHandle()->cast<Core::IProjectWindow>()->doc()->closeRequested();
        m_clientAddOn = nullptr;
    }

    QByteArray VstBridge::saveDataFromEditor() {
        qDebug() << "VstBridge: saveDataFromEditor";
        static char failValue[1] = {-128};
        static const QByteArray failValueData(failValue, 1);
        if(!m_clientAddOn) return failValueData;
        //TODO
        return "test";
    }

    bool VstBridge::openDataToEditor(const QByteArray &data) {
        qDebug() << "VstBridge: openDataToEditor";
        auto doc = new Core::DspxDocument(Core::DspxDocument::VSTMode);
        if(data.isEmpty()) {
            doc->makeNew("VST");
        } else {
            if (!doc->openRawData("VST", data)) {
                delete doc;
                return false;
            }
        }
        if(m_clientAddOn) emit m_clientAddOn->windowHandle()->cast<Core::IProjectWindow>()->doc()->closeRequested();
        Core::IWindow::create<Core::IProjectWindow>(doc);
        if (qApp->property("closeHomeOnOpen").toBool() && Core::ICore::instance()->windowSystem()->count() == 2) {
            auto iWin = Core::IHomeWindow::instance();
            
        }
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

    void VstBridge::notifySwitchAudioBuffer(qint64 position, int bufferSize, int channelCount, bool isPlaying, bool isRealtime) {
    }

} // Vst