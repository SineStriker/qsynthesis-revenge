//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_AUDIODRIVERMANAGER_H
#define CHORUSKIT_AUDIODRIVERMANAGER_H

#include "AudioDriver.h"

class AudioDriverManagerPrivate;

class AudioDriverManager: public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AudioDriverManager)
public:
    AudioDriverManager(QObject *parent = nullptr);
    ~AudioDriverManager();

    bool addAudioDriver(AudioDriver *driver);
    bool removeDriver(AudioDriver *driver);
    AudioDriver *driver(const QString &name) const;
    QStringList drivers() const;

signals:
    void driverAdded(AudioDriver *driver);
    void driverRemoved(AudioDriver *driver);

protected:
    AudioDriverManager(AudioDriverManagerPrivate &d, QObject *parent);
    QScopedPointer<AudioDriverManagerPrivate> d_ptr;
};



#endif // CHORUSKIT_AUDIODRIVERMANAGER_H
