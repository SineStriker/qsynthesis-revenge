//
// Created by Crs_1 on 2023/5/12.
//

#ifndef CHORUSKIT_DIFFSCOPEEDITOR_H
#define CHORUSKIT_DIFFSCOPEEDITOR_H

#include <QProcess>

#include <QMSystem.h>

#include "IpcReceiveChannel.h"
#include "IpcSendChannel.h"

namespace Vst {

    class DiffScopeEditor: public QObject {
        Q_OBJECT
    public:
        enum Status {
            Disconnected,
            Pending,
            Connected,
        };
        explicit DiffScopeEditor();

        bool start();
        void stop();
        Status status();

        IpcServer *mainServer();
        IpcServer *processingServer();
        IpcServer *dirtySettingServer();

        IpcSendChannel *mainChannel();
        IpcSendChannel *processingChannel();
        IpcReceiveChannel *dirtySettingChannel();
    signals:
        void statusChanged(Status status);
    private:
        Status m_status = Disconnected;
        QProcess proc;
        IpcServer *m_mainServer = nullptr;
        IpcServer *m_processingServer = nullptr;
        IpcServer *m_dirtySettingServer = nullptr;
        IpcSendChannel *m_mainChannel = nullptr;
        IpcSendChannel *m_processingChannel = nullptr;
        IpcReceiveChannel *m_dirtySettingChannel = nullptr;
    };

} // Vst

#endif // CHORUSKIT_DIFFSCOPEEDITOR_H
