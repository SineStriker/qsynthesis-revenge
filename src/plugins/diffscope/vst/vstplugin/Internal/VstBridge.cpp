//
// Created by Crs_1 on 2023/5/26.
//

#include "VstBridge.h"

namespace Vst {

    static void sineWave(double sampleRate, int position, int size, float *buf) {
        double freq = 440 / sampleRate;
        for (int x = 0; x < size; x++) {
            buf[x] = ::sin(2 * 3.14159265358979323846 * freq * (position + x));
        }
    }

    void VstBridge::testProcess(int64_t pos, int32_t size) {

    }
    VstBridge::VstBridge(QObject *parent): VstBridgeSource(parent) {
    }
} // Vst