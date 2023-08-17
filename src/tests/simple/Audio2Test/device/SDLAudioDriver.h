//
// Created by Crs_1 on 2023/7/11.
//

#ifndef CHORUSKIT_SDLAUDIODRIVER_H
#define CHORUSKIT_SDLAUDIODRIVER_H

#include "AudioDriver.h"

class SDLAudioDriverPrivate;
class SDLAudioDriverEventPoller;
class SDLAudioDevice;

class SDLAudioDriver: public AudioDriver {
    Q_OBJECT
    Q_DECLARE_PRIVATE(SDLAudioDriver)
    friend class SDLAudioDevice;
public:
    explicit SDLAudioDriver(QObject *parent = nullptr);
    ~SDLAudioDriver();

    bool initialize() override;
    void finalize() override;
    QStringList devices() const override;
    QString defaultDevice() const override;
    AudioDevice *createDevice(const QString &name) override;

    static QList<SDLAudioDriver *> getDrivers();

protected:
    SDLAudioDriver(SDLAudioDriverPrivate &d, QObject *parent);
    void addOpenedDevice(quint32 devId, SDLAudioDevice *dev);
    void removeOpenedDevice(quint32 devId);
};



#endif // CHORUSKIT_SDLAUDIODRIVER_H
