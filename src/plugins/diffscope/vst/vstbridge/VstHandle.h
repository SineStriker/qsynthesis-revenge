//
// Created by Crs_1 on 2023/5/28.
//

#ifndef CHORUSKIT_VSTHANDLE_H
#define CHORUSKIT_VSTHANDLE_H

#include "CommunicationHelper.h"
#include "ParameterTypes.h"
namespace Vst {

    class VstHandle {

    public:

        void *editorHelper = nullptr;

        Callbacks callbacks;

        QScopedPointer<CommunicationHelper> ch;

        QProcess editorProc;

        std::atomic<bool> isConnected = false;

        QSharedMemory sbuf;

        QScopedPointer<QSharedMemory> guardSharedMemory;

    };

} // Vst

#endif // CHORUSKIT_VSTHANDLE_H
