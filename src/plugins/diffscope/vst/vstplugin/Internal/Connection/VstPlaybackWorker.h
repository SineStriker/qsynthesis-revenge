//
// Created by Crs_1 on 2023/8/3.
//

#ifndef CHORUSKIT_VSTPLAYBACKWORKER_H
#define CHORUSKIT_VSTPLAYBACKWORKER_H

#include <QObject>

class QSharedMemory;

namespace Vst {
    class VstProcessData;
    class VstBufferSwitchData;
}

namespace Vst::Internal {

    class VstPlaybackWorker: public QObject {
        Q_OBJECT
        QAtomicInteger<bool> m_requestFinish = false;
        QSharedMemory *m_processBufferSharedMemory;
        QSharedMemory *m_processDataSharedMemory;
        VstProcessData *m_processData = nullptr;
        VstBufferSwitchData *m_bufferSwitchData = nullptr;
        QVector<float *> m_planarOutputData;
    public:
        VstPlaybackWorker(QSharedMemory *processDataSharedMemory, QSharedMemory *processBufferSharedMemory, QObject *parent = nullptr);
    public slots:
        void start();
        void quit();
        bool initialize(float sampleRate, int bufferSize, int channelCount);
        void finalize();
        bool work(bool isRealtime, bool isPlaying, qint64 position, int bufferSize, int channelCount, float *const *output);
    };

} // Internal

#endif // CHORUSKIT_VSTPLAYBACKWORKER_H
