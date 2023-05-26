#ifndef AUDIO_PLUGIN_EXAMPLE_PARAMETERTYPES_H
#define AUDIO_PLUGIN_EXAMPLE_PARAMETERTYPES_H

#include <cstdint>

namespace Vst {

    struct PlaybackParameters {
        bool isRealtime;
        bool isPlaying;
        int64_t projectTimeSamples;
        int32_t numSamples;
        int32_t numOutputs;
    };

    struct Callbacks {
        void (*setDirty)();
        void (*setError)(const char *error);
        void (*setStatus)(const char *status);
    };

}
#endif //AUDIO_PLUGIN_EXAMPLE_PARAMETERTYPES_H
