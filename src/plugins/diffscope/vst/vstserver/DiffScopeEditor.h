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
        IpcSendChannel *mainChannel();
        IpcSendChannel *processingChannel();
        IpcReceiveChannel *dirtySettingChannel();
    private:
        Status m_status = Disconnected;
        QProcess proc;
        IpcSendChannel *m_mainChannel;
        IpcSendChannel *m_processingChannel;
        IpcReceiveChannel *m_dirtySettingChannel;
    };

} // Vst

#endif // CHORUSKIT_DIFFSCOPEEDITOR_H
