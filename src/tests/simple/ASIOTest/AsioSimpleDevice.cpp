//
// Created by Crs_1 on 2023/6/27.
//

#include "AsioSimpleDevice.h"

#include "asiodrivers.h"
#include <QStringList>
#include "ASIOConvertSamples.h"

static AsioSimpleDevice *m_instance = nullptr;

AsioSimpleDevice::~AsioSimpleDevice() {
    finalize();
    m_instance = nullptr;
}

AsioSimpleDevice *AsioSimpleDevice::instance() {
    return m_instance;
}
void AsioSimpleDevice::setName(QString name) {
    m_name = name;
}

QString AsioSimpleDevice::name() const {
    return m_name;
}

bool AsioSimpleDevice::initAsioStaticData() {
    if(ASIOGetChannels(&m_spec.inputChannels, &m_spec.outputChannels) != ASE_OK) return false;
    if(ASIOGetBufferSize(&m_spec.minSize, &m_spec.maxSize, &m_spec.preferredSize, &m_spec.granularity) != ASE_OK) return false;
    if(ASIOGetSampleRate(&m_spec.sampleRate) != ASE_OK) return false;
    m_spec.postOutput = (ASIOOutputReady() == ASE_OK);
    return true;
}

bool AsioSimpleDevice::createAsioBuffers() {

    ASIOBufferInfo *info = m_spec.bufferInfos;

    m_spec.inputBuffers = std::min((long)AsioSimpleSpec::MaxInputChannels, m_spec.inputChannels);
    for(long i = 0; i < m_spec.inputBuffers; i++, info++) {
        info->isInput = ASIOTrue;
        info->channelNum = i;
        info->buffers[0] = info->buffers[1] = nullptr;
    }

    m_spec.outputBuffers = std::min((long)AsioSimpleSpec::MaxOutputChannels, m_spec.outputChannels);
    for(long i = 0; i < m_spec.outputBuffers; i++, info++) {
        info->isInput = ASIOFalse;
        info->channelNum = i;
        info->buffers[0] = info->buffers[1] = nullptr;
    }

    if(ASIOCreateBuffers(m_spec.bufferInfos, m_spec.inputBuffers + m_spec.outputBuffers, m_spec.preferredSize, &m_cb) != ASE_OK) return false;

    for (long i = 0; i < m_spec.inputBuffers + m_spec.outputBuffers; i++) {
        m_spec.channelInfos[i].channel = m_spec.bufferInfos[i].channelNum;
        m_spec.channelInfos[i].isInput = m_spec.bufferInfos[i].isInput;
        if (ASIOGetChannelInfo(&m_spec.channelInfos[i]) != ASE_OK) return false;
    }

    if(ASIOGetLatencies(&m_spec.inputLatency, &m_spec.outputLatency) != ASE_OK) return false;

    return true;
}

bool AsioSimpleDevice::initialize() {
    if(!m_asioDrivers->loadDriver(m_name.toLocal8Bit().data())) return false;
    m_isAsioLoaded = true;
    if (ASIOInit(&m_spec.driverInfo) != ASE_OK) return false;
    m_isAsioInitialized = true;
    if(!initAsioStaticData()) return false;
    if(!createAsioBuffers()) return false;
    m_isBufferCreated = true;
    emit initializationStatusChanged(true);
    emit sampleRateChanged(m_spec.sampleRate);
    return true;
}

bool AsioSimpleDevice::isInitialized() const {
    return m_isBufferCreated;
}

void AsioSimpleDevice::finalize() {
    stop();
    bool doEmit = false;
    if(m_isBufferCreated) {
        ASIODisposeBuffers();
        m_isBufferCreated = false;
        doEmit = true;
    }
    m_spec = {0};
    if(m_isAsioInitialized) {
#ifdef Q_OS_WINDOWS
        m_asioDrivers->removeCurrentDriver();
        theAsioDriver = nullptr;
#else
        ASIOExit();
#endif
        m_isAsioInitialized = false;
        m_isAsioLoaded = false;
    }
    emit initializationStatusChanged(false);
}
bool AsioSimpleDevice::start(const AsioSimplePlayCallback &callback) {
    m_callback = callback;
    m_callbackBuffer = new float[m_spec.preferredSize * m_spec.outputBuffers];
    memset(m_callbackBuffer, 0, m_spec.preferredSize * m_spec.outputBuffers * sizeof(float));
    if(ASIOStart() == ASE_OK) {
        m_isPlaying = true;
        emit playbackStatusChanged(true);
        return true;
    }
    return false;
}

bool AsioSimpleDevice::isStarted() const {
    return m_isPlaying;
}

void AsioSimpleDevice::stop() {
    if(m_isPlaying) {
        ASIOStop();
        m_isPlaying = false;
        emit playbackStatusChanged(false);
    }
}
AsioSimpleSpec AsioSimpleDevice::spec() const {
    return m_spec;
}
AsioSimpleDevice::AsioSimpleDevice(): m_asioDrivers(new AsioDrivers) {
    m_instance = this;
}
bool AsioSimpleDevice::setSampleRate(double sampleRate) {
    if(ASIOSetSampleRate(sampleRate) != ASE_OK) return false;
    m_spec.sampleRate = sampleRate;
    return true;
}

void AsioSimpleDevice::bufferSwitch(long index, ASIOBool processNow) {
    // the actual processing callback.
    // Beware that this is normally in a seperate thread, hence be sure that you take care
    // about thread synchronization. This is omitted here for simplicity.

    // as this is a "back door" into the bufferSwitchTimeInfo a timeInfo needs to be created
    // though it will only set the timeInfo.samplePosition and timeInfo.systemTime fields and the according flags
    ASIOTime  timeInfo;
    memset (&timeInfo, 0, sizeof (timeInfo));

    // get the time stamp of the buffer, not necessary if no
    // synchronization to other media is required
    if(ASIOGetSamplePosition(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == ASE_OK)
        timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

    bufferSwitchTimeInfo (&timeInfo, index, processNow);
}

#if NATIVE_INT64
#define ASIO64toDouble(a)  (a)
#else
const double twoRaisedTo32 = 4294967296.;
#define ASIO64toDouble(a)  ((a).lo + (a).hi * twoRaisedTo32)
#endif

static unsigned long get_sys_reference_time()
{	// get the system reference time
#if WINDOWS
    return timeGetTime();
#elif MAC
    static const double twoRaisedTo32 = 4294967296.;
    UnsignedWide ys;
    Microseconds(&ys);
    double r = ((double)ys.hi * twoRaisedTo32 + (double)ys.lo);
    return (unsigned long)(r / 1000.);
#endif
}

static void float32toInt24inPlace(float* buffer, long frames)
{
    double sc = 0x7fffffL + .49999;
    long a;
    char* b = (char*)buffer;
    char* aa = (char*)&a;

    while(--frames >= 0)
    {
        a = (long)((double)(*buffer++) * sc);
#if ASIO_LITTLE_ENDIAN
        *b++ = aa[0];
        *b++ = aa[1];
        *b++ = aa[2];
#else
        *b++ = aa[3];
        *b++ = aa[2];
        *b++ = aa[1];
#endif
    }
}

ASIOTime *AsioSimpleDevice::bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow) {
    // the actual processing callback.
    // Beware that this is normally in a seperate thread, hence be sure that you take care
    // about thread synchronization. This is omitted here for simplicity.
    static long processedSamples = 0;
    
    // store the timeInfo for later use
    m_instance->m_spec.tInfo = *timeInfo;

    // get the time stamp of the buffer, not necessary if no
    // synchronization to other media is required
    if (timeInfo->timeInfo.flags & kSystemTimeValid)
        m_instance->m_spec.nanoSeconds = ASIO64toDouble(timeInfo->timeInfo.systemTime);
    else
        m_instance->m_spec.nanoSeconds = 0;

    if (timeInfo->timeInfo.flags & kSamplePositionValid)
        m_instance->m_spec.samples = ASIO64toDouble(timeInfo->timeInfo.samplePosition);
    else
        m_instance->m_spec.samples = 0;

    if (timeInfo->timeCode.flags & kTcValid)
        m_instance->m_spec.tcSamples = ASIO64toDouble(timeInfo->timeCode.timeCodeSamples);
    else
        m_instance->m_spec.tcSamples = 0;

    // get the system reference time
    m_instance->m_spec.sysRefTime = get_sys_reference_time();

    // buffer size in samples
    long buffSize = m_instance->m_spec.preferredSize;

    // perform the processing
    QVector<float *> bufferList;
    for(int i = 0; i < m_instance->m_spec.outputBuffers; i++) {
        bufferList.append(m_instance->m_callbackBuffer + i * buffSize);
    }

    int outputCount = 0;
    for (int i = 0; i < m_instance->m_spec.inputBuffers + m_instance->m_spec.outputBuffers; i++)
    {

        if (m_instance->m_spec.bufferInfos[i].isInput == false)
        {
            // OK do processing for the outputs only
            int typeSize = 0;
            switch (m_instance->m_spec.channelInfos[i].type)
            {
                case ASIOSTFloat32LSB:		// IEEE 754 32 bit float, as found on Intel x86 architecture
                    typeSize = 4;
                    break;
                case ASIOSTInt32LSB:
                    typeSize = 4;
                    ASIOConvertSamples().float32toInt32inPlace(m_instance->m_callbackBuffer + (buffSize * outputCount), buffSize);
                    break;
                case ASIOSTInt24LSB:
                    typeSize = 3;
                    float32toInt24inPlace(m_instance->m_callbackBuffer + (buffSize * outputCount), buffSize);
                    break;
                case ASIOSTInt16LSB:
                    typeSize = 2;
                    ASIOConvertSamples().float32toInt16inPlace(m_instance->m_callbackBuffer + (buffSize * outputCount), buffSize);
                    break;

            }
            memcpy (
                m_instance->m_spec.bufferInfos[i].buffers[index],
                m_instance->m_callbackBuffer + (buffSize * outputCount++),
                buffSize * typeSize);
        }
    }
    m_instance->m_callback(buffSize, bufferList);

    // finally if the driver supports the ASIOOutputReady() optimization, do it here, all data are in place
    if (m_instance->m_spec.postOutput)
        ASIOOutputReady();

    return nullptr;
}

void AsioSimpleDevice::sampleRateChangedCb(ASIOSampleRate sRate) {
    // do whatever you need to do if the sample rate changed
    // usually this only happens during external sync.
    // Audio processing is not stopped by the driver, actual sample rate
    // might not have even changed, maybe only the sample rate status of an
    // AES/EBU or S/PDIF digital input at the audio device.
    // You might have to update time/sample related conversion routines, etc.
    emit m_instance->sampleRateChanged(sRate);
}
long AsioSimpleDevice::asioMessages(long selector, long value, void *message, double *opt) {
    // currently the parameters "value", "message" and "opt" are not used.
    long ret = 0;
    switch(selector)
    {
        case kAsioSelectorSupported:
            if(value == kAsioResetRequest
                || value == kAsioEngineVersion
                || value == kAsioResyncRequest
                || value == kAsioLatenciesChanged
                // the following three were added for ASIO 2.0, you don't necessarily have to support them
                || value == kAsioSupportsTimeInfo
                || value == kAsioSupportsTimeCode
                || value == kAsioSupportsInputMonitor)
                ret = 1L;
            break;
        case kAsioResetRequest:
            // defer the task and perform the reset of the driver during the next "safe" situation
            // You cannot reset the driver right now, as this code is called from the driver.
            // Reset the driver is done by completely destruct is. I.e. ASIOStop(), ASIODisposeBuffers(), Destruction
            // Afterwards you initialize the driver again.
            m_instance->m_spec.stopped;  // In this sample the processing will just stop
            ret = 1L;
            break;
        case kAsioResyncRequest:
            // This informs the application, that the driver encountered some non fatal data loss.
            // It is used for synchronization purposes of different media.
            // Added mainly to work around the Win16Mutex problems in Windows 95/98 with the
            // Windows Multimedia system, which could loose data because the Mutex was hold too long
            // by another thread.
            // However a driver can issue it in other situations, too.
            ret = 1L;
            break;
        case kAsioLatenciesChanged:
            // This will inform the host application that the drivers were latencies changed.
            // Beware, it this does not mean that the buffer sizes have changed!
            // You might need to update internal delay data.
            ret = 1L;
            break;
        case kAsioEngineVersion:
            // return the supported ASIO version of the host application
            // If a host applications does not implement this selector, ASIO 1.0 is assumed
            // by the driver
            ret = 2L;
            break;
        case kAsioSupportsTimeInfo:
            // informs the driver wether the asioCallbacks.bufferSwitchTimeInfo() callback
            // is supported.
            // For compatibility with ASIO 1.0 drivers the host application should always support
            // the "old" bufferSwitch method, too.
            ret = 1;
            break;
        case kAsioSupportsTimeCode:
            // informs the driver wether application is interested in time code info.
            // If an application does not need to know about time code, the driver has less work
            // to do.
            ret = 0;
            break;
    }
    return ret;
}

QStringList AsioSimpleDevice::drivers() const {
    QStringList drvList;
    QByteArray strBuf(64, 0);
#ifdef Q_OS_WINDOWS
    auto drvNum = m_asioDrivers->asioGetNumDev();
#else //Mac
    auto drvNum = m_asioDrivers->getNumFragments();
#endif
    for(long i = 0; i < m_asioDrivers->asioGetNumDev(); i++) {
#ifdef Q_OS_WINDOWS
        m_asioDrivers->asioGetDriverName(i, strBuf.data(), 32);
#else
        m_asioDrivers->getName(i, strBuf.data());
#endif
        drvList.append(strBuf);
    }
    return drvList;
}
