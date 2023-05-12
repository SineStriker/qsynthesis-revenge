//
// Created by Crs_1 on 2023/5/12.
//

#ifndef CHORUSKIT_IPCSENDCHANNEL_H
#define CHORUSKIT_IPCSENDCHANNEL_H

#include "IpcChannel.h"

namespace Vst {

    class IpcSendChannel : public IpcChannel {
        Q_OBJECT
    public:
        explicit IpcSendChannel(const QString &key, QObject *parent = nullptr);
        QByteArray send(quint8 signal, const QByteArray &data = {});
        bool allocate(int size);
    };

} // Vst

#endif // CHORUSKIT_IPCSENDCHANNEL_H
