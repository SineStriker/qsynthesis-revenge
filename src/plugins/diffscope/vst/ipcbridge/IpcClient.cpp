//
// Created by Crs_1 on 2023/5/13.
//

#include "IpcClient.h"

namespace Vst {
    IpcClient::IpcClient(const QString &key, QObject *parent) : IpcConnect(key, parent) {
        socket = new QLocalSocket(this);
        socket->setServerName(key);
        connect(socket, &QLocalSocket::connected, this, [=](){
            emit connected();
        });
        connect(socket, &QLocalSocket::disconnected, this, [=](){
            emit disconnected();
        });
    }
    IpcClient::~IpcClient() {
    }
    void IpcClient::abort() {
        QMutexLocker locker(&mutex);
        socket->disconnectFromServer();
    }
    bool IpcClient::open() {
        return socket->open();
    }
} // Vst