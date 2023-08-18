//
// Created by Crs_1 on 2023/8/17.
//

#ifndef CHORUSKIT_ASIOAUDIODRIVER_H
#define CHORUSKIT_ASIOAUDIODRIVER_H

#ifndef USE_FEATURE_ASIO
#   warning ASIO Feature is not enabled
#else

#include "AudioDriver.h"

class ASIOAudioDriverPrivate;

class ASIOAudioDriver: public AudioDriver {
    Q_OBJECT
    Q_DECLARE_PRIVATE(ASIOAudioDriver)
public:
    ASIOAudioDriver(QObject *parent = nullptr);
    ~ASIOAudioDriver();
    bool initialize() override;
    void finalize() override;
    QStringList devices() const override;
    QString defaultDevice() const override;
    AudioDevice *createDevice(const QString &name) override;

protected:
    ASIOAudioDriver(ASIOAudioDriverPrivate &d, QObject *parent);
};

#endif // USE_FEATURE_ASIO

#endif // CHORUSKIT_ASIOAUDIODRIVER_H
