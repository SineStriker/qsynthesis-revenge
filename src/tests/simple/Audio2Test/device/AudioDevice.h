//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIODEVICE_H
#define CHORUSKIT_AUDIODEVICE_H

#include "AudioDeviceCallback.h"
#include "utils/IAudioStream.h"
#include "utils/IErrorStringProvider.h"
#include "utils/INameProvider.h"

#include <QObject>

class AudioDevicePrivate;
class AudioDriver;

/**
 * @brief Base class for audio devices
 */
class AudioDevice: public QObject, public IAudioStream, public INameProvider, public IErrorStringProvider {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AudioDevice)
    friend class AudioDriver;
public:

    virtual ~AudioDevice();

    /**
     * Returns @c true if the device has been successfully initialized.
     *
     * The initialization state must be checked before opening the device.
     */
    bool isInitialized() const;

    /**
     * Gets the driver underlying this device.
     */
    AudioDriver *driver() const;

    /**
     * Gets the number of channels of this device.
     *
     * Note that this function will not work until this device is opened.
     *
     * @see open()
     */
    int channelCount() const;

    /**
     * Gets the number of active channels of this device. The return value is less or equal the number of all channels.
     *
     * Note that this function will not work until this device is opened.
     *
     * @see open(), channelCount()
     */
    int activeChannelCount() const;

    /**
     * Sets the maximum number of active channels of this device.
     *
     * Note that this function must be invoked when this device is closed. Invoking this function while this device is
     * opened is undefined behavior.
     */
    void setActiveChannelCount(int num);

    /**
     * Gets all buffer sizes that this device supports.
     */
    QList<qint64> availableBufferSizes() const;

    /**
     * Gets the most suitable buffer size for this device.
     */
    qint64 preferredBufferSize() const;

    /**
     * Gets all sample rates that this device supports.
     */
    QList<double> availableSampleRates() const;

    /**
     * Gets the most suitable sample rate for this device.
     */
    double preferredSampleRate() const;

    /**
     * Starts the device.
     *
     * Only after the device is opened can this function be invoked.
     *
     * Note that the ownership of AudioDeviceCallback object is not taken.
     *
     * @param audioDeviceCallback the callback for streaming audio data
     *
     * @return true if started successfully
     */
    virtual bool start(AudioDeviceCallback *audioDeviceCallback);

    /**
     * Returns @c true if the device is started.
     */
    bool isStarted() const;

    /**
     * Stops the device.
     */
    virtual void stop();

    /**
     * Locks the audio callback.
     *
     * Only lightweight work should be done while the audio device is locked. If any heavy work is going to be done, stop
     * the audio device first, do the work, and start it again.
     *
     * The audio device must be unlocked when it is going to stop or close, otherwise these functions will be blocked.
     */
    virtual void lock();

    /**
     * Unlock the audio callback.
     */
    virtual void unlock();

    /**
     * Opens the control panel of this device.
     * @return @c true if opened successfully, or @c false if the function fails or this device does not have control panel.
     */
    virtual bool openControlPanel();

    void close() override;

signals:

    /**
     * Emitted when the audio device is closed automatically due to internal reasons or closed manually.
     */
    void closed();

protected:
    explicit AudioDevice(QObject *parent = nullptr);
    AudioDevice(AudioDevicePrivate &d, QObject *parent);
    QScopedPointer<AudioDevicePrivate> d_ptr;

    /**
     * Sets the initialization state in the constructor.
     *
     * @see isInitialized()
     */
    void setIsInitialized(bool isInitialized);

    /**
     * Sets the driver underlying this device in the constructor.
     */
    void setDriver(AudioDriver *driver);

    /**
     * Sets the number of channel after the device is opened.
     */
    void setChannelCount(int channelCount);

    /**
     * Sets all buffer sizes that this device supports in the constructor.
     *
     * @see availableBufferSizes()
     */
    void setAvailableBufferSizes(const QList<qint64> &bufferSizes);

    /**
     * Sets the most suitable buffer size for this device in the constructor.
     *
     * @see preferredBufferSize()
     */
    void setPreferredBufferSize(qint64 bufferSize);

    /**
     * Sets all sample rates that this device supports in the constructor.
     *
     * @see availableBufferSizes()
     */
    void setAvailableSampleRates(const QList<double> &sampleRates);

    /**
     * Sets the most suitable sample rate for this device in the constructor.
     *
     * @see preferredBufferSize()
     */
    void setPreferredSampleRate(double sampleRate);

};



#endif // CHORUSKIT_AUDIODEVICE_H
