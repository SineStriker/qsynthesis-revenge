//
// Created by Crs_1 on 2023/5/26.
//

#ifndef CHORUSKIT_VSTBRIDGE_H
#define CHORUSKIT_VSTBRIDGE_H

#include "rep_VstBridge_source.h"

class QLocalSocket;
class QSharedMemory;
class QSystemSemaphore;

namespace Vst::Internal {

    class VstHelper;
    class VstPlaybackWorker;

    class VstBridge: public VstBridgeSource {
        Q_OBJECT
        friend class VstHelper;
        explicit VstBridge(QObject *parent = nullptr);
    public:

        ~VstBridge();
        static VstBridge *instance();

        bool initializeVst() override;
        void finalizeVst() override;

        QByteArray saveDataFromEditor() override;
        bool openDataToEditor(const QByteArray &data) override;

        void showWindow() override;
        void hideWindow() override;

        bool initializeProcess(int channelCount, int maxBufferSize, double sampleRate) override;
        void notifySwitchAudioBuffer(bool isRealtime, bool isPlaying, qint64 position, int bufferSize,
                                     int channelCount) override;
        void finalizeProcess() override;

    private:
        QLocalSocket *m_alivePipe;
        QSharedMemory *m_processDataSharedMemory;
        QThread *m_vstPlaybackWorkerThread;
        VstPlaybackWorker *m_worker;
    };

} // Vst

#endif // CHORUSKIT_VSTBRIDGE_H
