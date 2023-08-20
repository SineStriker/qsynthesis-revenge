//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIODRIVER_H
#define CHORUSKIT_AUDIODRIVER_H

#include <QObject>

#include "utils/IErrorStringProvider.h"
#include "utils/INameProvider.h"

class AudioDriverPrivate;
class AudioDevice;

/**
 * @brief Base class for audio drivers that underlay audio devices
 */
class AudioDriver: public QObject, public INameProvider, public IErrorStringProvider {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AudioDriver)
public:
    explicit AudioDriver(QObject *parent = nullptr);
    virtual ~AudioDriver();

    /**
     * Initializes current audio driver.
     *
     * The driver must be initialized first before getting the device list or creating device.
     *
     * @return @c true if the initialization is successful
     */
    virtual bool initialize();

    /**
     * Terminates current audio driver.
     *
     * After the driver is terminated, the behavior of created devices is undefined, therefore the life cycle of drivers
     * and devices should be managed properly.
     */
    virtual void finalize();

    /**
     * Returns @c true if the audio driver is initialized.
     */
    bool isInitialized() const;

    /**
     * Gets the names of devices under this driver.
     */
    virtual QStringList devices() const = 0;

    /**
     * Gets the name of default device under this driver.
     * @return the device name, or null string if this driver does not support specifying default device
     */
    virtual QString defaultDevice() const;

    /**
     * Creates an audio device.
     *
     * The parent of the device created is set to this object, and use @c delete to deallocate.
     *
     * @c nullptr will be returned if the function fails to create the device with the specified name. Note that the audio
     * device is not guaranteed to work, and use @link AudioDevice::isInitialized() to check whether it can work or not.
     *
     * @see AudioDevice, AudioDevice::isInitialized()
     */
    virtual AudioDevice *createDevice(const QString &name) = 0;

signals:

    /**
     * Emitted when the list of devices is changed. Not all drivers support this feature.
     */
    void deviceChanged();

protected:
    explicit AudioDriver(AudioDriverPrivate &d, QObject *parent);
    QScopedPointer<AudioDriverPrivate> d_ptr;
};



#endif // CHORUSKIT_AUDIODRIVER_H
