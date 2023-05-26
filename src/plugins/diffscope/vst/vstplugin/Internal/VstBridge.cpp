//
// Created by Crs_1 on 2023/5/26.
//

#include "VstBridge.h"
#include <QSharedMemory>

namespace Vst {

    static void sineWave(double sampleRate, int position, int size, float *buf) {
        double freq = 440 / sampleRate;
        for (int x = 0; x < size; x++) {
            buf[x] = ::sin(2 * 3.14159265358979323846 * freq * (position + x));
        }
    }

    static QSharedMemory sbuf("77F6E993-671E-4283-99BE-C1CD1FF5C09E");

    bool VstBridge::testProcess(int64_t pos, int32_t size) {
        if(!sbuf.isAttached()) sbuf.attach();
        sbuf.lock();
        sineWave(48000, pos, size, static_cast<float *>(sbuf.data()));
        sbuf.unlock();
        return true;
    }
    VstBridge::VstBridge(QObject *parent): VstBridgeSource(parent) {
    }
} // Vst