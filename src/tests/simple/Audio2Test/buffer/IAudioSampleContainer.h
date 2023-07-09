//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_IAUDIOSAMPLECONTAINER_H
#define CHORUSKIT_IAUDIOSAMPLECONTAINER_H



#include "IAudioSampleProvider.h"
class IAudioSampleContainer : public IAudioSampleProvider {
public:
    virtual float &sampleAt(int channel, int pos) = 0;

    void setSampleRange(int destChannel, int destStartPos, int length, const IAudioSampleProvider &src, int srcChannel,
                        int srcStartPos);
    void setSampleRange(const IAudioSampleProvider &src);

    void addSampleRange(int destChannel, int destStartPos, int length, const IAudioSampleProvider &src, int srcChannel,
                        int srcStartPos, float gain = 1);
    void addSampleRange(const IAudioSampleProvider &src, float gain = 1);

    void gainSampleRange(int destChannel, int destStartPos, int length, float gain);
    void gainSampleRange(int destChannel, float gain);
    void gainSampleRange(float gain);

    void clear(int destChannel, int destStartPos, int length);
    void clear(int destChannel);
    void clear();
};



#endif // CHORUSKIT_IAUDIOSAMPLECONTAINER_H
