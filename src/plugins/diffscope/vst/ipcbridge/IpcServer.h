//
// Created by Crs_1 on 2023/5/12.
//

#ifndef CHORUSKIT_IPCSERVER_H
#define CHORUSKIT_IPCSERVER_H

#include "IpcConnect.h"
#include <QLocalServer>
#include <QLocalSocket>
#include <QMutex>
#include <QSharedMemory>
namespace Vst {

    class IpcServer : public IpcConnect {
        Q_OBJECT
    public:
        IpcServer(const QString &key, QObject *parent);
        ~IpcServer();
        void abort() override;
    protected:
        QLocalServer server;
    };

} // Vst

#endif // CHORUSKIT_IPCSERVER_H
