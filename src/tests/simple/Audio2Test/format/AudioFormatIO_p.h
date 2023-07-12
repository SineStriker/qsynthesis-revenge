//
// Created by Crs_1 on 2023/7/12.
//

#ifndef CHORUSKIT_AUDIOFORMATIO_P_H
#define CHORUSKIT_AUDIOFORMATIO_P_H

#include "AudioFormatIO.h"

class AudioFormatIOPrivate {
    Q_DECLARE_PUBLIC(AudioFormatIO)
public:
    AudioFormatIO *q_ptr;
    QIODevice *stream = nullptr;
    QScopedPointer<SndfileHandle> sf;
    AudioFormatIO::OpenMode openMode = QIODevice::NotOpen;

    qint64 sfVioGetFilelen();
    qint64 sfVioSeek(qint64 offset, int whence);
    qint64 sfVioRead(void *ptr, qint64 count);
    qint64 sfVioWrite(const void *ptr, qint64 count);
    qint64 sfVioTell();
};

#endif // CHORUSKIT_AUDIOFORMATIO_P_H
