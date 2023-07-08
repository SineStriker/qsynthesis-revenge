//
// Created by Crs_1 on 2023/7/8.
//

#ifndef CHORUSKIT_IAUDIOSAMPLECONTAINER_H
#define CHORUSKIT_IAUDIOSAMPLECONTAINER_H



class IAudioSampleContainer {
public:

    virtual ~IAudioSampleContainer() = default;

    virtual float &sampleAt(int channel, int pos) = 0;
    virtual float constSampleAt(int channel, int pos) const = 0;

    virtual int channelCount() const = 0;
    virtual int sampleCount() const = 0;

    void setSampleRange(int destChannel, int destStartPos, int length, const IAudioSampleContainer &src, int srcChannel,
                        int srcStartPos);
    void setSampleRange(const IAudioSampleContainer &src);

    void addSampleRange(int destChannel, int destStartPos, int length, const IAudioSampleContainer &src, int srcChannel,
                        int srcStartPos, float gain = 1);
    void addSampleRange(const IAudioSampleContainer &src, float gain = 1);

    void gainSampleRange(int destChannel, int destStartPos, int length, float gain);
    void gainSampleRange(int destChannel, float gain);
    void gainSampleRange(float gain);

    void clear(int destChannel, int destStartPos, int length);
    void clear(int destChannel);
    void clear();

    float magnitude(int channel, int startPos, int length) const;
    float magnitude(int channel) const;

    float rms(int channel, int startPos, int length) const;
    float rms(int channel) const;

};



#endif // CHORUSKIT_IAUDIOSAMPLECONTAINER_H
