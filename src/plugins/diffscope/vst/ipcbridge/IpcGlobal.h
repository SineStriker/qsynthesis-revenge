//
// Created by Crs_1 on 2023/5/13.
//

#ifndef CHORUSKIT_IPCGLOBAL_H
#define CHORUSKIT_IPCGLOBAL_H

namespace Vst {

    const quint8 DETACH_SHARED_MEMORY_SIGNAL = 0xff;
    const quint8 ATTACH_SHARED_MEMORY_SIGNAL = 0xfe;

    const quint32 HEADER_REQUEST = 0x11451400;
    const quint32 HEADER_RESPONSE = 0x19260817;
}

#endif // CHORUSKIT_IPCGLOBAL_H
