//
// Created by Crs_1 on 2023/5/13.
//

#ifndef CHORUSKIT_IPCCONNECT_H
#define CHORUSKIT_IPCCONNECT_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QMutex>
#include <QSharedMemory>
namespace Vst {

    class IpcSendChannel;
    class IpcReceiveChannel;

    class IpcConnect : public QObject {
        Q_OBJECT
    public:
        explicit IpcConnect(const QString &key, QObject *parent = nullptr);
        bool lockSharedMemory();
        bool unlockSharedMemory();
        void *sharedData();
        virtual void abort() = 0;
        const void *sharedData() const;
    signals:
        void connected();
        void disconnected();
    protected:
        friend class IpcSendChannel;
        friend class IpcReceiveChannel;
        QString key;
        QLocalSocket *socket = nullptr;
        QSharedMemory sharedMemory;
        QMutex mutex;
    };

} // Vst

#endif // CHORUSKIT_IPCCONNECT_H
