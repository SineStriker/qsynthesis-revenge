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
    QList<quint32> acceptableSampleRates();
    bool canAcceptSampleRate(quint32 sampleRate);
    virtual bool addSource(IAudioSource *src);
    QList<IAudioSource *> sources();
    virtual bool removeSource(IAudioSource *source);
protected:
    void addAcceptableSampleRate(quint32 sampleRate);
    void removeAcceptableSampleRate(quint32 sampleRate);
private:
    QSet<quint32> m_acceptableSampleRates;
    QMChronSet<IAudioSource *> m_sources;
};



#endif // CHORUSKIT_AUDIOBUS_H
