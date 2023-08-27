//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIODRIVERMANAGER_H
#define CHORUSKIT_AUDIODRIVERMANAGER_H

#include "AudioDriver.h"

class AudioDriverManagerPrivate;

/**
 * @brief A registry of audio drivers
 *
 * This object is designed for dynamatic registration of audio drivers for DiffScope plugins.
 */
class AudioDriverManager: public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AudioDriverManager)
public:
    AudioDriverManager(QObject *parent = nullptr);
    ~AudioDriverManager();

    /**
     * Adds audio driver to this manager.
     *
     * Note that the parent of driver object is set to this object.
     *
     * @return @c true if added successfully, @c false if already exists.
     */
    bool addAudioDriver(AudioDriver *driver);

    /**
     * Remove audio driver from this manager.
     *
     * Note that the parent of driver object is set to @c nullptr.
     *
     * @return @c true if removed successfully, @c false if not exists.
     */
    bool removeDriver(AudioDriver *driver);

    /**
     * Gets the driver with a specified name.
     */
    AudioDriver *driver(const QString &name) const;

    /**
     * Gets a list of names of all drivers.
     */
    QStringList drivers() const;

    /**
     * Creates an audio driver manager with all built-in drivers.
     */
    static AudioDriverManager *createBuiltInDriverManager(QObject *parent = nullptr);

signals:
    void driverAdded(AudioDriver *driver);
    void driverRemoved(AudioDriver *driver);

protected:
    AudioDriverManager(AudioDriverManagerPrivate &d, QObject *parent);
    QScopedPointer<AudioDriverManagerPrivate> d_ptr;
};



#endif // CHORUSKIT_AUDIODRIVERMANAGER_H
