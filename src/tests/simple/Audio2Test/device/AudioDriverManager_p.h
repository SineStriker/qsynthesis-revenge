//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIODRIVERMANAGER_P_H
#define CHORUSKIT_AUDIODRIVERMANAGER_P_H

#include "AudioDriverManager.h"

#include "QMChronMap.h"

class AudioDriverManagerPrivate {
    Q_DECLARE_PUBLIC(AudioDriverManager)
public:
    AudioDriverManager *q_ptr;
    QMChronMap<QString, AudioDriver *> driverDict;
};

#endif // CHORUSKIT_AUDIODRIVERMANAGER_P_H
