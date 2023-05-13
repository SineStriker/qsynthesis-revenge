//
// Created by Crs_1 on 2023/5/12.
//

#include "DiffScopeEditor.h"
#include "IpcConnect.h"

const QString VST_UUID = "77F6E993-671E-4283-99BE-C1CD1FF5C09E";

namespace Vst {
    void DiffScopeEditor::start()  {
        if(proc.isOpen()) emit statusChanged(m_status);
        QFile configFile(QMFs::appDataPath() + "/ChorusKit/DiffScope/vstconfig.txt");
        configFile.open(QIODevice::ReadOnly);
        configFile.readLine();
        configFile.readLine();
        QString path = configFile.readLine();
        if(!QFileInfo(path).isExecutable()) emit statusChanged(m_status);
        m_mainServer = new IpcServer(VST_UUID, this);
        m_processingServer = new IpcServer(VST_UUID + "processing", this);
        m_dirtySettingServer = new IpcServer(VST_UUID + "dirty_setting", this);
        m_mainChannel = new IpcSendChannel(m_mainServer);
        m_processingChannel = new IpcSendChannel(m_processingServer);
        m_dirtySettingChannel = new IpcReceiveChannel(m_dirtySettingServer);
        auto argv = QStringList({"vst", VST_UUID});
        proc.setProgram(path);
        proc.setArguments(argv);
        connect(&proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus){
            m_status = Disconnected;
            emit statusChanged(m_status);
        });
        connect(m_mainServer, &IpcServer::connected, this, [=](){
            m_status = Connected;
            emit statusChanged(m_status);
        });
        connect(m_mainServer, &IpcServer::disconnected, this, [=](){
            m_status = Disconnected;
            emit statusChanged(m_status);
        });
        if(proc.startDetached()) {
            m_status = Pending;
            emit statusChanged(m_status);
        } else {
            m_status = Disconnected;
            emit statusChanged(m_status);
        }
    }
    DiffScopeEditor::DiffScopeEditor() {
    }
    void DiffScopeEditor::stop() {
        if(m_mainServer) m_mainServer->abort();
        if(m_processingServer) m_processingServer->abort();
        if(m_dirtySettingServer) m_dirtySettingServer->abort();
    }
    DiffScopeEditor::Status DiffScopeEditor::status() {
        return m_status;
    }
    IpcServer *DiffScopeEditor::mainServer() {
        return m_mainServer;
    }
    IpcServer *DiffScopeEditor::processingServer() {
        return m_processingServer;
    }
    IpcServer *DiffScopeEditor::dirtySettingServer() {
        return m_dirtySettingServer;
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