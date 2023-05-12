//
// Created by Crs_1 on 2023/5/12.
//

#include "IpcSendChannel.h"
#include "IpcChannel.h"
#include <QDataStream>

const quint8 DETACH_SHARED_MEMORY_SIGNAL = 0xff;
const quint8 ATTACH_SHARED_MEMORY_SIGNAL = 0xfe;
const quint32 HEADER_REQUEST = 0x11451400;
const quint32 HEADER_RESPONSE = 0x19260817;

namespace Vst {
    IpcSendChannel::IpcSendChannel(const QString &key, QObject *parent) : IpcChannel(key, parent) {
        sharedMemory.create(1024);
    }
    QByteArray IpcSendChannel::send(quint8 signal, const QByteArray &data) {
        QMutexLocker locker(&mutex);
        if(!socket) return {};
        QByteArray request;
        QDataStream outgoing(&request, QIODevice::WriteOnly);
        outgoing << HEADER_REQUEST << signal << data;
        socket->write(request);
        socket->waitForReadyRead();
        auto response = socket->readAll();
        QDataStream incoming(response);
        quint32 header;
        incoming >> header;
        if(header != HEADER_RESPONSE) return {};
        QByteArray payload;
        incoming >> payload;
        return payload;
    }
    bool IpcSendChannel::allocate(int size) {
        QMutexLocker locker(&mutex);
        if(!sharedMemory.isAttached()) {
            return sharedMemory.create(size);
        } else {
            sharedMemory.lock();
            send(DETACH_SHARED_MEMORY_SIGNAL);
            sharedMemory.unlock();
            sharedMemory.detach();
            if(sharedMemory.create(size)) {
                send(ATTACH_SHARED_MEMORY_SIGNAL);
                return true;
            } else {
                return false;
            }
        }
    }
} // Vst