//
// Created by Crs_1 on 2023/8/3.
//

#ifndef CHORUSKIT_VSTPLAYBACKWORKER_H
#define CHORUSKIT_VSTPLAYBACKWORKER_H

#include <QObject>

namespace Vst::Internal {

    class VstPlaybackWorker: public QObject {
        Q_OBJECT
    public:
        explicit VstPlaybackWorker(QObject *parent = nullptr);
    public slots:
        void work(bool isRealtime, bool isPlaying, qint64 position, int bufferSize, int channelCount, float *const *output);
    signals:
        void requestWork(bool isRealtime, bool isPlaying, qint64 position, int bufferSize, int channelCount, float *const *output);
        void workFinished(bool isSuccessful);
    };

} // Internal

#endif // CHORUSKIT_VSTPLAYBACKWORKER_H
