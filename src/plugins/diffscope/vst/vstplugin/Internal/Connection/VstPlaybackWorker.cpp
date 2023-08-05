//
// Created by Crs_1 on 2023/8/3.
//

#include <cmath>

#include "VstPlaybackWorker.h"

#include "VstHelper.h"

namespace Vst::Internal {

    VstPlaybackWorker::VstPlaybackWorker(QObject *parent) : QObject(parent) {
        connect(this, &VstPlaybackWorker::requestWork, this, &VstPlaybackWorker::work);
    }

    static qint64 pos = 0;

    static void genSineWave(qint64 position, qint64 size, float freq, float sampleRate, float *output) {
        double omega = 2 * 3.14159265358979323846 * freq / sampleRate;
        for(int i = 0; i < size; i++) {
            output[i] = sin(omega * (position + i));
        }
    }

    void VstPlaybackWorker::work(bool isRealtime, bool isPlaying, qint64 position, int bufferSize, int channelCount, float *const *output) {
        // TODO
        if(isPlaying) {
            genSineWave(position, bufferSize, 440, VstHelper::instance()->connectionStatus.sampleRate, output[0]);
            pos = 0;
        } else {
            genSineWave(pos, bufferSize, 880, VstHelper::instance()->connectionStatus.sampleRate, output[0]);
            pos += bufferSize;
        }
        emit workFinished(true);
    }
} // Internal