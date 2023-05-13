//
// Created by Crs_1 on 2023/5/12.
//

#include "IpcReceiveChannel.h"
#include "IpcConnect.h"
#include <QDataStream>
#include "IpcGlobal.h"

namespace Vst {
    IpcReceiveChannel::IpcReceiveChannel(IpcConnect *ipcConnect) : QObject(ipcConnect), ipcConnect(ipcConnect) {
        connect(ipcConnect, &IpcConnect::connected, this, [=](){
            ipcConnect->sharedMemory.attach();
            connect(ipcConnect->socket, &QLocalSocket::readyRead, this, &IpcReceiveChannel::_received);
        });
    }
    void IpcReceiveChannel::_received() {
        auto request = ipcConnect->socket->readAll();
        if(request.size() < 5) return;
        QDataStream incoming(request);
        quint32 header;
        quint8 signal;
        incoming >> header >> signal;
        if(header != HEADER_REQUEST) return;
        if(signal == DETACH_SHARED_MEMORY_SIGNAL) {
            ipcConnect->sharedMemory.detach();
        } else if(signal == ATTACH_SHARED_MEMORY_SIGNAL) {
            ipcConnect->sharedMemory.attach();
        } else {
            QByteArray payload;
            incoming >> payload;
            QByteArray ret;
            emit received(signal, payload, ret);
            QByteArray response;
            QDataStream outgoing(&response, QIODevice::WriteOnly);
            outgoing << HEADER_RESPONSE << ret;
            ipcConnect->socket->write(response);
        }
    }
} // Vst