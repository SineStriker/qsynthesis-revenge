//
// Created by Crs_1 on 2023/5/12.
//

#ifndef CHORUSKIT_IPCCHANNEL_H
#define CHORUSKIT_IPCCHANNEL_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QMutex>
#include <QSharedMemory>
namespace Vst {

    class IpcChannel: public QObject {
        Q_OBJECT
    public:
        explicit IpcChannel(const QString &key, QObject *parent = nullptr);
        ~IpcChannel();
        void abort();
        bool lockSharedMemory();
        bool unlockSharedMemory();
        void *sharedData();
        const void *sharedData() const;
    signals:
        void connected();
        void disconnected();
    protected:
        QString key;
        QLocalServer server;
        QLocalSocket *socket = nullptr;
        QSharedMemory sharedMemory;
        QMutex mutex;
    };

} // Vst

#endif // CHORUSKIT_IPCCHANNEL_H
