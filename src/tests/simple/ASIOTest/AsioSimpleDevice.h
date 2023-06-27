#ifndef CHORUSKIT_ASIOSIMPLEDEVICE_H
#define CHORUSKIT_ASIOSIMPLEDEVICE_H

#define NOMINMAX
#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <functional>
#include <asiosys.h>
#include <asio.h>

struct IASIO;
extern IASIO *theAsioDriver;

struct AsioSimpleSpec {

    enum {
        MaxInputChannels = 32,
        MaxOutputChannels = 32,
    };

    // ASIOInit()
    ASIODriverInfo driverInfo;

    // ASIOGetChannels()
    long           inputChannels;
    long           outputChannels;

    // ASIOGetBufferSize()
    long           minSize;
    long           maxSize;
    long           preferredSize;
    long           granularity;

    // ASIOGetSampleRate()
    ASIOSampleRate sampleRate;

    // ASIOOutputReady()
    bool           postOutput;

    // ASIOGetLatencies ()
    long           inputLatency;
    long           outputLatency;

    // ASIOCreateBuffers ()
    long inputBuffers;	// becomes number of actual created input buffers
    long outputBuffers;	// becomes number of actual created output buffers
    ASIOBufferInfo bufferInfos[MaxInputChannels + MaxOutputChannels]; // buffer info's

    // ASIOGetChannelInfo()
    ASIOChannelInfo channelInfos[MaxInputChannels + MaxOutputChannels]; // channel info's
    // The above two arrays share the same indexing, as the data in them are linked together

    // Information from ASIOGetSamplePosition()
    // data is converted to double floats for easier use, however 64 bit integer can be used, too
    double         nanoSeconds;
    double         samples;
    double         tcSamples;	// time code samples

    // bufferSwitchTimeInfo()
    ASIOTime       tInfo;			// time info state
    unsigned long  sysRefTime;      // system reference time, when bufferSwitch() was called

    // Signal the end of processing in this example
    bool           stopped;
};

typedef std::function<void(int i, long size, float *buf)> AsioSimplePlayCallback;

class AsioDrivers;

class AsioSimpleDevice: public QObject {
    Q_OBJECT
public:
    ~AsioSimpleDevice();

    static AsioSimpleDevice *instance();

    void setName(QString name);
    QString name() const;
    bool initialize();
    bool isInitialized() const;
    void finalize();
    bool start(const AsioSimplePlayCallback &callback);
    bool isStarted() const;
    void stop();
    bool setSampleRate(double sampleRate);

    AsioSimpleSpec spec() const;

    QStringList drivers() const;

signals:
    void initializationStatusChanged(bool initializationStatus);
    void playbackStatusChanged(bool playbackStatus);

private:
    friend int main(int, char **);
    AsioSimpleDevice();
    QString m_name;
    QScopedPointer<AsioDrivers> m_asioDrivers;
    bool m_isAsioLoaded = false;
    bool m_isAsioInitialized = false;
    bool m_isBufferCreated = false;
    bool m_isPlaying = false;
    AsioSimpleSpec m_spec = {0};
    bool initAsioStaticData();
    bool createAsioBuffers();
    ASIOCallbacks m_cb = {&bufferSwitch, &sampleRateChanged, &asioMessages, &bufferSwitchTimeInfo};

    AsioSimplePlayCallback m_callback;

    //ASIO callbacks
    static void bufferSwitch(long index, ASIOBool processNow);
    static ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow);
    static void sampleRateChanged(ASIOSampleRate sRate);
    static long asioMessages(long selector, long value, void* message, double* opt);
};



#endif // CHORUSKIT_ASIOSIMPLEDEVICE_H
