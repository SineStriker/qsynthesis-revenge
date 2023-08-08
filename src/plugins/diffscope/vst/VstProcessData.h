//
// Created by Crs_1 on 2023/8/8.
//

#ifndef CHORUSKIT_VSTPROCESSDATA_H
#define CHORUSKIT_VSTPROCESSDATA_H

namespace Vst {
    struct VstProcessData {
        enum Flag {
            ProcessInitializationRequest,
            ProcessInitializationResponse,
            ProcessInitializationError,
            BufferSwitchRequest,
            BufferSwitchFinished,
            BufferSwitchError,
        };
        float sampleRate;
        int bufferSize;
        int channelCount;
        Flag flag;
    };
    struct VstBufferSwitchData {
        bool isRealtime;
        bool isPlaying;
        qint64 position;
        int size;
        int channelCount;
    };
}
#endif // CHORUSKIT_VSTPROCESSDATA_H
