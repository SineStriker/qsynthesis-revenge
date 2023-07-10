//
// Created by Crs_1 on 2023/7/11.
//

#ifndef CHORUSKIT_SDLAUDIODRIVER_H
#define CHORUSKIT_SDLAUDIODRIVER_H

#include "AudioDriver.h"

class SDLAudioDriverPrivate;

class SDLAudioDriver: public AudioDriver {
    Q_OBJECT
    Q_DECLARE_PRIVATE(SDLAudioDriver)
public:
    explicit SDLAudioDriver(QObject *parent = nullptr);

    bool initialize() override;
    void finalize() override;
    QStringList devices() override;
    QString defaultDevice() override;
    AudioDevice *createDevice(const QString &name, QObject *parent) override;

protected:
    SDLAudioDriver(SDLAudioDriverPrivate &d, QObject *parent);
};



#endif // CHORUSKIT_SDLAUDIODRIVER_H
