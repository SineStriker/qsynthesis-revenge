//
// Created by Crs_1 on 2023/5/26.
//

#include "VstBridge.h"

#include <QSharedMemory>
#include <QLocalSocket>

#include <Window/IProjectWindow.h>
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
        return {};
    }

    bool VstBridge::openDataToEditor(const QByteArray &data) {
        qDebug() << "VstBridge: openDataToEditor";
        auto docMgr = Core::ICore::instance()->documentSystem();
        //TODO temporarily do this
        auto recentFiles = docMgr->recentFiles();
        if(!recentFiles.empty()) {
            auto spec = docMgr->docType("org.ChorusKit.dspx");
            spec->open(recentFiles[0], nullptr);
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