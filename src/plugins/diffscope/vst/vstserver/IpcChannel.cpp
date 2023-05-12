//
// Created by Crs_1 on 2023/5/12.
//

#include "IpcChannel.h"
#include <QDataStream>
Vst::IpcChannel::IpcChannel(const QString &key, QObject *parent): QObject(parent), key(key) {
    server.setMaxPendingConnections(1);
    server.listen(key);
    connect(&server, &QLocalServer::newConnection, this, [=](){
        socket = server.nextPendingConnection();
        server.close();
        connect(socket, &QLocalSocket::disconnected, this, [=](){ emit disconnected(); });
        emit connected();
    });
    sharedMemory.setKey(key);
}
Vst::IpcChannel::~IpcChannel() {
}
void Vst::IpcChannel::abort() {
    QMutexLocker locker(&mutex);
    if(socket) {
        socket->disconnectFromServer();
        socket->deleteLater();
        socket = nullptr;
        server.listen(key);
    }
}
bool Vst::IpcChannel::lockSharedMemory() {
    return sharedMemory.lock();
}
bool Vst::IpcChannel::unlockSharedMemory() {
    return sharedMemory.unlock();
}
void *Vst::IpcChannel::sharedData() {
    return sharedMemory.data();
}
const void *Vst::IpcChannel::sharedData() const {
    return sharedMemory.constData();
}
