//
// Created by Crs_1 on 2023/7/15.
//

#ifndef CHORUSKIT_TRANSPORTAUDIOSOURCEWRITER_P_H
#define CHORUSKIT_TRANSPORTAUDIOSOURCEWRITER_P_H

#include "TransportAudioSourceWriter.h"

#include "source/TransportAudioSource_p.h"

class TransportAudioSourceWriterPrivate {
    Q_DECLARE_PUBLIC(TransportAudioSourceWriter);
public:
    TransportAudioSourceWriter *q_ptr;
    TransportAudioSource *src;
    TransportAudioSourcePrivate *pSrc;
    AudioFormatIO *outFile;
    qint64 startPos;
    qint64 length;
    QAtomicInteger<bool> stopRequested = false;
};

#endif // CHORUSKIT_TRANSPORTAUDIOSOURCEWRITER_P_H
