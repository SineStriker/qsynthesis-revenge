//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIODRIVER_H
#define CHORUSKIT_AUDIODRIVER_H

#include <QObject>

#include "utils/IErrorStringProvider.h"
#include "utils/INameProvider.h"

class AudioDriverPrivate;
class AudioDevice;

/* abstract */ class AudioDriver: public QObject, public INameProvider, public IErrorStringProvider {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AudioDriver)
public:
    explicit AudioDriver(QObject *parent = nullptr);
    virtual ~AudioDriver();

    virtual bool initialize();
    virtual void finalize();
    bool isInitialized() const;

    virtual QStringList devices() const = 0;
    virtual QString defaultDevice() const;

    virtual AudioDevice *createDevice(const QString &name) = 0;

signals:
    void deviceChanged();

protected:
    explicit AudioDriver(AudioDriverPrivate &d, QObject *parent);
    QScopedPointer<AudioDriverPrivate> d_ptr;
};



#endif // CHORUSKIT_AUDIODRIVER_H
