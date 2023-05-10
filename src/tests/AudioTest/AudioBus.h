//
// Created by Crs_1 on 2023/5/5.
//

#ifndef CHORUSKIT_AUDIOBUS_H
#define CHORUSKIT_AUDIOBUS_H

#include <QSet>
#include "Collections/QMChronSet.h"
#include "IAudioSource.h"

class AudioBus {
public:
    quint32 acceptableSampleRate() const;
    virtual bool addSource(IAudioSource *src);
    QList<IAudioSource *> sources() const;
    virtual bool removeSource(IAudioSource *source);
protected:
    bool setAcceptableSampleRate(quint32 sampleRate);
    QMChronSet<IAudioSource *> m_sources;
private:
    quint32 m_acceptableSampleRate;

};



#endif // CHORUSKIT_AUDIOBUS_H
