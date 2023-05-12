//
// Created by Crs_1 on 2023/5/12.
//

#include "DiffScopeEditor.h"

const QString VST_UUID = "77F6E993-671E-4283-99BE-C1CD1FF5C09E";

namespace Vst {
    bool DiffScopeEditor::start()  {
        if(proc.isOpen()) return true;
        QFile configFile(QMFs::appDataPath() + "/ChorusKit/DiffScope/vstconfig.txt");
        configFile.open(QIODevice::ReadOnly);
        configFile.readLine();
        configFile.readLine();
        if(!configFile.canReadLine()) return false;
        QString path = configFile.readLine();
        if(!QFileInfo(path).isExecutable()) return false;
        m_mainChannel = new IpcSendChannel(VST_UUID, this);
        m_processingChannel = new IpcSendChannel(VST_UUID + "processing", this);
        m_dirtySettingChannel = new IpcReceiveChannel(VST_UUID + "dirty_setting", this);
        auto argv = QStringList({"vst", VST_UUID});
        proc.setProgram(path);
        proc.setArguments(argv);
        connect(&proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus){
            m_status = Disconnected;
        });
        connect(m_mainChannel, &IpcSendChannel::connected, this, [=](){
            m_status = Connected;
        });
        connect(m_mainChannel, &IpcSendChannel::disconnected, this, [=](){
            m_status = Disconnected;
        });
        if(proc.startDetached()) {
            m_status = Pending;
            return true;
        } else {
            return false;
        }
    }
    DiffScopeEditor::DiffScopeEditor() {
    }
    void DiffScopeEditor::stop() {
        if(m_mainChannel) m_mainChannel->abort();
        if(m_processingChannel) m_processingChannel->abort();
        if(m_dirtySettingChannel) m_dirtySettingChannel->abort();
    }
    DiffScopeEditor::Status DiffScopeEditor::status() {
        return m_status;
    }
    IpcSendChannel *DiffScopeEditor::mainChannel() {
        return m_mainChannel;
    }
    IpcSendChannel *DiffScopeEditor::processingChannel() {
        return m_processingChannel;
    }
    IpcReceiveChannel *DiffScopeEditor::dirtySettingChannel() {
        return m_dirtySettingChannel;
    }
} // Vst