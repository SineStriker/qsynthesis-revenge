//
// Created by Crs_1 on 2023/5/28.
//

#ifndef CHORUSKIT_DIFFSCOPEVSTBRIDGE_H
#define CHORUSKIT_DIFFSCOPEVSTBRIDGE_H

#include <IVstBridge.h>
#include <IEditorCallbacks.h>

#include "CommunicationHelper.h"

namespace Vst {

    class DiffScopeVstBridge: public IVstBridge {

    public:
        bool initialize() override;
        void terminate() override;
        void bindEditorCallbacks(IEditorCallbacks *iEditorCallbacks) override;
        void showWindow() override;
        void hideWindow() override;
        bool initializeProcess(int channelCount, int maxBufferSize, double sampleRate) override;
        bool processPlayback(bool isRealtime, bool isPlaying, int64_t position, int size, int channelCount,
                             float *const *outputs) override;
        void finalizeProcess() override;
        void stateChanged(int size, const char *data) override;
        bool stateWillSave(int &size, const char *&data) override;
        void stateSaved(const char *data) override;

    private:
        IEditorCallbacks *callbacks;

        QScopedPointer<CommunicationHelper> ch;
        QProcess editorProc;

        std::atomic<bool> isConnected = false;
        std::atomic<bool> isPending = false;

        QSharedMemory sbuf;
        QScopedPointer<QSharedMemory> guardSharedMemory;
        QLocalServer alivePipe;

        QByteArray cachedState;

    private:
        bool checkSingleton();
        void releaseSingleton();

        void replicaInitialized();
        void replicaNotInitialized();

        void stateChanged(const QByteArray &data);
    };

} // Vst

#endif // CHORUSKIT_DIFFSCOPEVSTBRIDGE_H
