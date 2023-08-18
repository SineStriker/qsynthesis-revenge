//
// Created by Crs_1 on 2023/7/10.
//

#include "AudioDriverManager.h"
#include "AudioDriverManager_p.h"
#include "SDLAudioDriver.h"

#ifdef USE_FEATURE_ASIO
#   include "ASIOAudioDriver.h"
#endif

AudioDriverManager::AudioDriverManager(QObject *parent): AudioDriverManager(*new AudioDriverManagerPrivate, parent) {
}
AudioDriverManager::~AudioDriverManager() {
}
bool AudioDriverManager::addAudioDriver(AudioDriver *driver) {
    Q_D(AudioDriverManager);
    if(!driver) {
        qWarning() << "AudioDriverManager: driver to add is null.";
        return false;
    }
    auto driverName = driver->name();
    if(d->driverDict.contains(driverName)) {
        qWarning() << QString("AudioDriverManager: driver '%1' is already added.").arg(driverName);
        return false;
    }
    driver->setParent(this);
    d->driverDict.append(driverName, driver);
    emit driverAdded(driver);
    return true;
}
bool AudioDriverManager::removeDriver(AudioDriver *driver) {
    Q_D(AudioDriverManager);
    if(!driver) {
        qWarning() << "AudioDriverManager: driver to remove is null.";
        return false;
    }
    auto driverName = driver->name();
    auto it = d->driverDict.find(driverName);
    if(it == d->driverDict.end()) {
        qWarning() << QString("AudioDriverManager: driver '%1' does not exist.").arg(driverName);
        return false;
    }
    driver->setParent(nullptr);
    d->driverDict.erase(it);
    emit driverRemoved(driver);
    return true;
}
AudioDriver *AudioDriverManager::driver(const QString &name) const {
    Q_D(const AudioDriverManager);
    return d->driverDict.value(name, nullptr);
}
QStringList AudioDriverManager::drivers() const {
    Q_D(const AudioDriverManager);
    return d->driverDict.keys();
}
AudioDriverManager::AudioDriverManager(AudioDriverManagerPrivate &d, QObject *parent): QObject(parent), d_ptr(&d) {
    d.q_ptr = this;
}

AudioDriverManager *AudioDriverManager::createBuiltInDriverManager(QObject *parent) {
    auto drvMgr = new AudioDriverManager(parent);
    for(auto drv: SDLAudioDriver::getDrivers()) {
        drvMgr->addAudioDriver(drv);
    }
#ifdef USE_FEATURE_ASIO
    drvMgr->addAudioDriver(new ASIOAudioDriver);
#endif
    return drvMgr;
}
