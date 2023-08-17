//
// Created by Crs_1 on 2023/8/17.
//

#ifndef CHORUSKIT_ASIOAUDIODRIVER_P_H
#define CHORUSKIT_ASIOAUDIODRIVER_P_H

#ifdef USE_FEATURE_ASIO

#include "ASIOAudioDriver.h"
#include "AudioDriver_p.h"

#include <qt_windows.h>

static const int MAXPATHLEN = 512;
static const int MAXDRVNAMELEN = 128;

struct ASIODeviceSpec {
    CLSID clsid;
    QString driverName;
};

class ASIOAudioDriverPrivate : public AudioDriverPrivate {
    Q_DECLARE_PUBLIC(ASIOAudioDriver)
    QList<ASIODeviceSpec> asioDriverSpecs;
    void createDriverSpec(HKEY hkey, char *keyName);
};

#endif // USE_FEATURE_ASIO

#endif // CHORUSKIT_ASIOAUDIODRIVER_P_H
