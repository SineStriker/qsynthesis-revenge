//
// Created by Crs_1 on 2023/5/28.
//

#ifndef CHORUSKIT_VSTHANDLE_H
#define CHORUSKIT_VSTHANDLE_H

#include <IVstBridge.h>
#include <IEditorCallbacks.h>

#include "CommunicationHelper.h"

namespace Vst {

    class VstHandle: public IVstBridge {

    public:

        void *editorHelper = nullptr;

        Callbacks callbacks;

        QScopedPointer<CommunicationHelper> ch;

        QProcess editorProc;

        std::atomic<bool> isConnected = false;

        std::atomic<bool> isPending = false;

        QSharedMemory sbuf;

        QScopedPointer<QSharedMemory> guardSharedMemory;

        QLocalServer alivePipe;

    };

} // Vst

#endif // CHORUSKIT_VSTHANDLE_H
