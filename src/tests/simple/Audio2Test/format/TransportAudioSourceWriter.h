//
// Created by Crs_1 on 2023/7/15.
//

#ifndef CHORUSKIT_TRANSPORTAUDIOSOURCEWRITER_H
#define CHORUSKIT_TRANSPORTAUDIOSOURCEWRITER_H

#include <QObject>

class TransportAudioSourceWriterPrivate;

class TransportAudioSource;
class AudioFormatIO;

/**
 * @brief The TransportAudioSourceWriter class provides interfaces to write audio.
 *
 * The audio is read from a TransportAudioSource object, and write to an AudioFormatIO object.
 *
 * @see TransportAudioSource, AudioFormatIO
 */
class TransportAudioSourceWriter: public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(TransportAudioSourceWriter)
public:
    /**
     * Constructor.
     *
     * TransportAudioSource object and the AudioFormatIO object must be opened before writing is started, and closed manually
     * after writing.
     *
     * Note that this object does not take the ownership of both objects.
     */
    TransportAudioSourceWriter(TransportAudioSource *src, AudioFormatIO *outFile, qint64 startPos, qint64 length);
    ~TransportAudioSourceWriter();

public slots:
    /**
     * Starts writing.
     *
     * Note that the function should be called from another thread to avoid from blocking the main thread. It is recommended
     * to move this object to another thread using [QObject::moveToThread](https://doc.qt.io/qt-5/qobject.html#moveToThread)()
     * and connect signals and slots across different threads.
     *
     * @see [QThread](https://doc.qt.io/qt-5/qthread.html)
     */
    void start();

    /**
     * Interrupts the writing process. This function is thread-safe.
     *
     * Note that do not use queued connection to connect this slot to any signal, otherwise this function will not be invoked
     * before completed, since the thread is blocked while writing.
     */
    void interrupt();

signals:
    /**
     * Emitted when each block have been written.
     * @param percentage the percentage of the part finished.
     */
    void percentageUpdated(float percentage);

    /**
     * Emitted when writing is completed.
     *
     * Note that this signal is emitted after percentageUpdated().
     */
    void completed();

    /**
     * Emitted when writing finishes, either interrupted or completed.
     *
     * Note that this signal is emitted after completed() or interrupted().
     */
    void finished(bool isCompleted);

    /**
     * Emitted when writing is interrupted.
     */
    void interrupted();

protected:
    QScopedPointer<TransportAudioSourceWriterPrivate> d_ptr;
    explicit TransportAudioSourceWriter(TransportAudioSourceWriterPrivate &d);
};




#endif // CHORUSKIT_TRANSPORTAUDIOSOURCEWRITER_H
