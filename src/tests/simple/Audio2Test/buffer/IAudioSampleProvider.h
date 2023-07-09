//
// Created by Crs_1 on 2023/7/9.
//

#ifndef CHORUSKIT_IAUDIOSAMPLEPROVIDER_H
#define CHORUSKIT_IAUDIOSAMPLEPROVIDER_H



class IAudioSampleProvider {
public:
    virtual ~IAudioSampleProvider() = default;
    virtual float constSampleAt(int channel, int pos) const = 0;
    virtual int channelCount() const = 0;
    virtual int sampleCount() const = 0;
    float magnitude(int channel, int startPos, int length) const;
    float magnitude(int channel) const;
    float rms(int channel, int startPos, int length) const;
    float rms(int channel) const;
};



#endif // CHORUSKIT_IAUDIOSAMPLEPROVIDER_H
