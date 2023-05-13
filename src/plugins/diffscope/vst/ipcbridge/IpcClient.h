//
// Created by Crs_1 on 2023/5/13.
//

#ifndef CHORUSKIT_IPCCLIENT_H
#define CHORUSKIT_IPCCLIENT_H

#include "IpcConnect.h"
namespace Vst {

    class IpcClient : public IpcConnect {
        Q_OBJECT
    public:
        IpcClient(const QString &key, QObject *parent);
        ~IpcClient();
        void abort() override;
        bool open();
    };

} // Vst

#endif // CHORUSKIT_IPCCLIENT_H
