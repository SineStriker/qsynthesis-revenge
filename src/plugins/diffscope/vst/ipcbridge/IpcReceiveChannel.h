//
// Created by Crs_1 on 2023/5/12.
//

#ifndef CHORUSKIT_IPCRECEIVECHANNEL_H
#define CHORUSKIT_IPCRECEIVECHANNEL_H

#include "IpcServer.h"
namespace Vst {

    class IpcReceiveChannel: public QObject {
        Q_OBJECT
    public:
        explicit IpcReceiveChannel(IpcConnect *ipcConnect);
    signals:
        void received(quint8 signal, const QByteArray &data, QByteArray &ret);
    private slots:
        void _received();
    private:
        IpcConnect *ipcConnect;
    };

} // Vst

#endif // CHORUSKIT_IPCRECEIVECHANNEL_H
