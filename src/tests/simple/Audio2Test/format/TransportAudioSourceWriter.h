//
// Created by Crs_1 on 2023/7/15.
//

#ifndef CHORUSKIT_TRANSPORTAUDIOSOURCEWRITER_H
#define CHORUSKIT_TRANSPORTAUDIOSOURCEWRITER_H

#include <QObject>

class TransportAudioSourceWriterPrivate;

class TransportAudioSource;
class AudioFormatIO;

class TransportAudioSourceWriter: public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(TransportAudioSourceWriter)
public:
    TransportAudioSourceWriter(TransportAudioSource *src, AudioFormatIO *outFile, qint64 startPos, qint64 length);
    ~TransportAudioSourceWriter();

public slots:
    void start();
    void interrupt();

signals:
    void percentageUpdated(int percentage);
    void completed();
    void finished(bool isCompleted);
    void interrupted();

protected:
    QScopedPointer<TransportAudioSourceWriterPrivate> d_ptr;
    explicit TransportAudioSourceWriter(TransportAudioSourceWriterPrivate &d);
};




#endif // CHORUSKIT_TRANSPORTAUDIOSOURCEWRITER_H
