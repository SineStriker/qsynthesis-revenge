//
// Created by Crs_1 on 2023/7/15.
//

#include "TransportAudioSourceWriter.h"
#include "TransportAudioSourceWriter_p.h"

#include <QMutex>

#include "buffer/InterleavedAudioDataWrapper.h"
#include "AudioFormatIO.h"
#include "source/TransportAudioSource.h"

TransportAudioSourceWriter::TransportAudioSourceWriter(TransportAudioSource *src, AudioFormatIO *outFile, qint64 startPos, qint64 length): TransportAudioSourceWriter(*new TransportAudioSourceWriterPrivate) {
    Q_D(TransportAudioSourceWriter);
    d->src = src;
    d->pSrc = src->d_func();
    d->outFile = outFile;
    d->startPos = startPos;
    d->length = length;
}
TransportAudioSourceWriter::TransportAudioSourceWriter(TransportAudioSourceWriterPrivate &d): d_ptr(&d) {
    d.q_ptr = this;
}
TransportAudioSourceWriter::~TransportAudioSourceWriter() {
}

void TransportAudioSourceWriter::start() {
    Q_D(TransportAudioSourceWriter);
    {
        QMutexLocker locker(&d->pSrc->mutex);

        qint64 curPos = d->pSrc->position;
        qint64 blockSize = d->src->bufferSize();
        int chCnt = d->outFile->channels();

        auto *p = new float[blockSize * chCnt];
        memset(p, 0, sizeof(float) * blockSize * chCnt);
        InterleavedAudioDataWrapper buf(p, chCnt, blockSize);

        d->pSrc->_q_positionAboutToChange(d->startPos);
        d->pSrc->position = d->startPos;
        if (d->pSrc->src)
            d->pSrc->src->setNextReadPosition(d->pSrc->position);

        qint64 framesToRead = d->length;
        while (!d->stopRequested && framesToRead > 0) {
            qint64 readLength = std::min(framesToRead, blockSize);
            if (d->pSrc->src)
                d->pSrc->src->read({&buf, 0, readLength});
            framesToRead -= readLength;
            d->outFile->write(p, blockSize);
            d->pSrc->_q_positionAboutToChange(d->pSrc->position + readLength);
            d->pSrc->position += readLength;
            emit percentageUpdated(100.0 * (d->length - framesToRead) / d->length);
        }

        delete[] p;

        d->pSrc->_q_positionAboutToChange(curPos);
        d->pSrc->position = curPos;
        if (d->pSrc->src)
            d->pSrc->src->setNextReadPosition(curPos);
    }


    if(d->stopRequested) {
        emit interrupted();
        emit finished(false);
    } else {
        emit completed();
        emit finished(true);
    }
}
void TransportAudioSourceWriter::interrupt() {
    Q_D(TransportAudioSourceWriter);
    d->stopRequested = true;
}