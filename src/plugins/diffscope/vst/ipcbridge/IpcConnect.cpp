//
// Created by Crs_1 on 2023/5/13.
//

#include "IpcConnect.h"
#include "IpcServer.h"
#include <QDataStream>
Vst::IpcConnect::IpcConnect(const QString &key, QObject *parent): QObject(parent), key(key), mutex(QMutex::Recursive) {
    sharedMemory.setKey(key);
}
bool Vst::IpcConnect::lockSharedMemory() {
    return sharedMemory.lock();
}
bool Vst::IpcConnect::unlockSharedMemory() {
    return sharedMemory.unlock();
}
void *Vst::IpcConnect::sharedData() {
    return sharedMemory.data();
}
const void *Vst::IpcConnect::sharedData() const {
    return sharedMemory.constData();
}