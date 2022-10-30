#ifndef WAVEOBJECT_H
#define WAVEOBJECT_H

#include <QObject>
#include <QIODevice>
#include <QAtomicInt>

#include <list>

class WaveObject : public QObject {
    Q_OBJECT
public:
    explicit WaveObject(QObject *parent = nullptr);
    ~WaveObject();

    QIODevice *dev;

    QString fileName;

    QAtomicInt exitFlag;

public slots:
    void start();

signals:
    void finished();
};

#endif // WAVEOBJECT_H
