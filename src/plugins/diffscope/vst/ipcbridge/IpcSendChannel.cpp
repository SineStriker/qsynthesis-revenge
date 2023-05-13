//
// Created by Crs_1 on 2023/5/12.
//

#include "IpcSendChannel.h"
#include "IpcConnect.h"
#include "IpcServer.h"
#include <QDataStream>
#include "IpcGlobal.h"

namespace Vst {
    IpcSendChannel::IpcSendChannel(IpcConnect *ipcConnect) : QObject(ipcConnect), ipcConnect(ipcConnect) {
        ipcConnect->sharedMemory.create(1024);
    }
    QByteArray IpcSendChannel::send(quint8 signal, const QByteArray &data) {
        QMutexLocker locker(&ipcConnect->mutex);
        if(!ipcConnect->socket) return {};
        QByteArray request;
        QDataStream outgoing(&request, QIODevice::WriteOnly);
        outgoing << HEADER_REQUEST << signal << data;
        ipcConnect->socket->write(request);
        ipcConnect->socket->flush();
        ipcConnect->socket->waitForReadyRead();
        auto response = ipcConnect->socket->readAll();
        QDataStream incoming(response);
        quint32 header;
        incoming >> header;
        if(header != HEADER_RESPONSE) return {};
        QByteArray payload;
        incoming >> payload;
        return payload;
    }
    bool IpcSendChannel::allocate(int size) {
        QMutexLocker locker(&ipcConnect->mutex);
        if(!ipcConnect->sharedMemory.isAttached()) {
            return ipcConnect->sharedMemory.create(size);
        } else {
            ipcConnect->sharedMemory.lock();
            send(DETACH_SHARED_MEMORY_SIGNAL);
            ipcConnect->sharedMemory.unlock();
            ipcConnect->sharedMemory.detach();
            if(ipcConnect->sharedMemory.create(size)) {
                send(ATTACH_SHARED_MEMORY_SIGNAL);
                return true;
            } else {
                return false;
            }
        }
    }
} // Vst