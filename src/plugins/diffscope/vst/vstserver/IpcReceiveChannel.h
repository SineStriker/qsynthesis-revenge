//
// Created by Crs_1 on 2023/5/12.
//

#ifndef CHORUSKIT_IPCRECEIVECHANNEL_H
#define CHORUSKIT_IPCRECEIVECHANNEL_H

#include "IpcChannel.h"
namespace Vst {

    class IpcReceiveChannel: public IpcChannel {
        Q_OBJECT
    public:
        explicit IpcReceiveChannel(const QString &key, QObject *parent = nullptr);
    signals:
        void received(quint8 signal, const QByteArray &data, QByteArray &ret);
    private slots:
        void _received();
    };

} // Vst

#endif // CHORUSKIT_IPCRECEIVECHANNEL_H
