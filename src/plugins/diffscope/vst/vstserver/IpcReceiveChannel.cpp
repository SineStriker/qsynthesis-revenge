//
// Created by Crs_1 on 2023/5/12.
//

#include "IpcReceiveChannel.h"
#include <QDataStream>

const quint8 DETACH_SHARED_MEMORY_SIGNAL = 0xff;
const quint8 ATTACH_SHARED_MEMORY_SIGNAL = 0xfe;
const quint32 HEADER_REQUEST = 0x11451400;
const quint32 HEADER_RESPONSE = 0x19260817;

namespace Vst {
    IpcReceiveChannel::IpcReceiveChannel(const QString &key, QObject *parent) : IpcChannel(key, parent) {
        connect(this, &IpcReceiveChannel::connected, this, [=](){
            sharedMemory.attach();
            connect(socket, &QLocalSocket::readyRead, this, &IpcReceiveChannel::_received);
        });
    }
    void IpcReceiveChannel::_received() {
        auto request = socket->readAll();
        if(request.size() < 5) return;
        QDataStream incoming(request);
        quint32 header;
        quint8 signal;
        incoming >> header >> signal;
        if(header != HEADER_REQUEST) return;
        if(signal == DETACH_SHARED_MEMORY_SIGNAL) {
            sharedMemory.detach();
        } else if(signal == ATTACH_SHARED_MEMORY_SIGNAL) {
            sharedMemory.attach();
        } else {
            QByteArray payload;
            incoming >> payload;
            QByteArray ret;
            emit received(signal, payload, ret);
            QByteArray response;
            QDataStream outgoing(&response, QIODevice::WriteOnly);
            outgoing << HEADER_RESPONSE << ret;
            socket->write(response);
        }
    }
} // Vst