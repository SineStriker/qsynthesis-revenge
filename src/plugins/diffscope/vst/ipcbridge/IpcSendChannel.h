//
// Created by Crs_1 on 2023/5/12.
//

#ifndef CHORUSKIT_IPCSENDCHANNEL_H
#define CHORUSKIT_IPCSENDCHANNEL_H

#include "IpcServer.h"

namespace Vst {

    class IpcSendChannel : public QObject {
        Q_OBJECT
    public:
        explicit IpcSendChannel(IpcConnect *ipcConnect);
        QByteArray send(quint8 signal, const QByteArray &data = {});
        bool allocate(int size);
    private:
        IpcConnect *ipcConnect;
    };

} // Vst

#endif // CHORUSKIT_IPCSENDCHANNEL_H
