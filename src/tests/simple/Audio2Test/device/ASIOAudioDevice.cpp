//
// Created by Crs_1 on 2023/8/17.
//
#ifdef USE_FEATURE_ASIO

#include "ASIOAudioDevice.h"
#include "ASIOAudioDevice_p.h"

#include <QDebug>

#include "ASIOAudioDriver.h"
#include "buffer/AudioBuffer.h"
#include "utils/AudioSampleConverter.h"
#include "source/AudioSource.h"

static ASIOAudioDevice *m_device = nullptr;

static const QList<double> COMMON_SAMPLE_RATES = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000, 128000, 176400, 192000, 256000, 352800, 384000};

ASIOAudioDevice::ASIOAudioDevice(const QString &name, IASIO *iasio, ASIOAudioDriver *driver)
    : ASIOAudioDevice(*new ASIOAudioDevicePrivate, driver) {
    Q_D(ASIOAudioDevice);
    if(m_device) {
        qWarning() << "ASIOAudioDevice: Duplicated instance is not supported.";
        setErrorString("Duplicated instance is not supported.");
        return;
    }
    m_device = this;
    setName(name);
    setDriver(driver);
    d->iasio = iasio;
    if(!d->iasio->init(nullptr)) {
        qWarning() << "ASIOAudioDevice: Initialization failed" << name;
        d->iasio->getErrorMessage(d->errorMessageBuffer);
        setErrorString(d->errorMessageBuffer);
        return;
    }

    long numInputChannels, numOutputChannels;
    if(d->iasio->getChannels(&numInputChannels, &numOutputChannels) != ASE_OK)
        return;
    setChannelCount(numOutputChannels);

    long minSize, preferredSize, maxSize, granularity;
    if(d->iasio->getBufferSize(&minSize, &maxSize, &preferredSize, &granularity) != ASE_OK)
        return;
    setPreferredBufferSize(preferredSize);
    QList<qint64> bufferSizeList;
    if(granularity <= 0 || minSize > maxSize) {
        bufferSizeList.append(preferredSize);
    } else {
        for (long s = minSize; s <= maxSize; s += granularity) {
            bufferSizeList.append(s);
        }
    }
    setAvailableBufferSizes(bufferSizeList);

    double currentSampleRate;
    if(d->iasio->getSampleRate(&currentSampleRate) != ASE_OK)
        return;
    setPreferredSampleRate(currentSampleRate);
    QList<double> sampleRateList;
    for(auto sr: COMMON_SAMPLE_RATES) {
        if(d->iasio->canSampleRate(sr) == ASE_OK) {
            sampleRateList.append(sr);
        }
    }
    setAvailableSampleRates(sampleRateList);

    d->postOutput = (d->iasio->outputReady() == ASE_OK);

    d->isInitialized = true;
}
ASIOAudioDevice::ASIOAudioDevice(ASIOAudioDevicePrivate &d, QObject *parent): AudioDevice(d, parent) {
}
ASIOAudioDevice::~ASIOAudioDevice() {
    Q_D(ASIOAudioDevice);
    ASIOAudioDevice::close();
    d->iasio->Release();
    if(m_device == this)
        m_device = nullptr;
}

bool ASIOAudioDevice::open(qint64 bufferSize, double sampleRate) {
    if(!isInitialized())
        return false;
    Q_D(ASIOAudioDevice);
    ASIOAudioDevice::stop();
    if(d->iasio->setSampleRate(sampleRate) != ASE_OK) return false;

    for(int i = 0; i < activeChannelCount(); i++) {
        d->bufferInfoList.append({
            ASIOFalse,
            i,
            {nullptr, nullptr},
        });
        ASIOChannelInfo channelInfo = {i, ASIOFalse};
        if(d->iasio->getChannelInfo(&channelInfo) != ASE_OK)
            return false;
        d->channelInfoList.append(channelInfo);
    }
    if(d->iasio->createBuffers(d->bufferInfoList.data(), activeChannelCount(), bufferSize, &d->callbacks) != ASE_OK)
        return false;
    return IAudioStream::open(bufferSize, sampleRate);
}
void ASIOAudioDevice::close() {
    if(!isInitialized())
        return;
    Q_D(ASIOAudioDevice);
    ASIOAudioDevice::stop();
    d->iasio->disposeBuffers();
    d->bufferInfoList.clear();
    d->channelInfoList.clear();
    IAudioStream::close();
}

static AudioDeviceCallback *m_audioDeviceCallback = nullptr;
static AudioBuffer audioBuffer;

bool ASIOAudioDevice::start(AudioDeviceCallback *audioDeviceCallback) {
    if(!isInitialized())
        return false;
    Q_D(ASIOAudioDevice);
    QMutexLocker locker(&d->mutex);
    m_audioDeviceCallback = audioDeviceCallback;
    audioBuffer.resize(activeChannelCount(), bufferSize());
    m_audioDeviceCallback->deviceWillStartCallback(this);
    d->iasio->start();
    return AudioDevice::start(audioDeviceCallback);
}
void ASIOAudioDevice::stop() {
    if(!isInitialized())
        return;
    Q_D(ASIOAudioDevice);
    QMutexLocker locker(&d->mutex);
    d->iasio->stop();
    if(m_audioDeviceCallback) m_audioDeviceCallback->deviceStoppedCallback();
    audioBuffer.resize(0, 0);
    m_audioDeviceCallback = nullptr;
    AudioDevice::stop();
}

void ASIOAudioDevice::lock() {
    Q_D(ASIOAudioDevice);
    d->mutex.lock();
}
void ASIOAudioDevice::unlock() {
    Q_D(ASIOAudioDevice);
    d->mutex.unlock();
}

void ASIOAudioDevicePrivate::bufferSwitch(long index, ASIOBool processNow) {
    ASIOTime t = {};
    bufferSwitchTimeInfo(&t, index, processNow);
}
void ASIOAudioDevicePrivate::sampleRateDidChange(ASIOSampleRate sRate) {
    // TODO work on this
}
long ASIOAudioDevicePrivate::asioMessage(long selector, long value, void *message, double *opt) {
    // TODO work out on this
    // currently the parameters "value", "message" and "opt" are not used.
    long ret = 0;
    switch(selector) {
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
            qDebug() << "kAsioResetRequest";
            ret = 1L;
            break;
        case kAsioResyncRequest:
            // This informs the application, that the driver encountered some non fatal data loss.
            // It is used for synchronization purposes of different media.
            // Added mainly to work around the Win16Mutex problems in Windows 95/98 with the
            // Windows Multimedia system, which could loose data because the Mutex was hold too long
            // by another thread.
            // However a driver can issue it in other situations, too.
            qDebug() << "kAsioResyncRequest";
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

static void convertBuffer(void *dest, const float *src, qint64 length, ASIOSampleType type) {
    switch(type) {
        case ASIOSTInt16LSB:
            AudioSampleConverter::convertFloatToInt16(dest, src, length, true);
            break;
        case ASIOSTInt16MSB:
            AudioSampleConverter::convertFloatToInt16(dest, src, length, false);
            break;
        case ASIOSTInt24LSB:
            AudioSampleConverter::convertFloatToInt24(dest, src, length, true);
            break;
        case ASIOSTInt24MSB:
            AudioSampleConverter::convertFloatToInt24(dest, src, length, false);
            break;
        case ASIOSTInt32LSB:
            AudioSampleConverter::convertFloatToInt32(dest, src, length, true);
            break;
        case ASIOSTInt32MSB:
            AudioSampleConverter::convertFloatToInt32(dest, src, length, false);
            break;
        case ASIOSTFloat32LSB:
            AudioSampleConverter::convertFloatToFloat32(dest, src, length, true);
            break;
        case ASIOSTFloat32MSB:
            AudioSampleConverter::convertFloatToFloat32(dest, src, length, false);
            break;
        case ASIOSTFloat64LSB:
            AudioSampleConverter::convertFloatToFloat64(dest, src, length, true);
            break;
        case ASIOSTFloat64MSB:
            AudioSampleConverter::convertFloatToFloat64(dest, src, length, false);
            break;
        default:
            // unsupported sample type
            Q_ASSERT(false);
    }
}

ASIOTime *ASIOAudioDevicePrivate::bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow) {
    QMutexLocker locker(&m_device->d_func()->mutex);
    if(m_audioDeviceCallback) {
        m_audioDeviceCallback->workCallback(&audioBuffer);
    }
    for(int i = 0; i < audioBuffer.channelCount(); i++) {
        convertBuffer(m_device->d_func()->bufferInfoList[i].buffers[index], audioBuffer.constData(i), audioBuffer.sampleCount(), m_device->d_func()->channelInfoList[i].type);
    }
    audioBuffer.clear();
    if(m_device->d_func()->postOutput) {
        m_device->d_func()->iasio->outputReady();
    }
    return nullptr;
}

bool ASIOAudioDevice::openControlPanel() {
    Q_D(ASIOAudioDevice);
    return d->iasio->controlPanel() == ASE_OK;
}

#endif // USE_FEATURE_ASIO