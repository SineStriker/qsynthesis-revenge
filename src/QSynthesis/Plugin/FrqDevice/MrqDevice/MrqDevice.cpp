#include "MrqDevice.h"

MrqDevice::MrqDevice(QObject *parent) : IFrqDevice(parent) {
}

MrqDevice::~MrqDevice() {
}

bool MrqDevice::loadEntry(const QString dirname, const QString &entry, QFrqFile *out,
                          const QMap<QString, QVariant> &args) {
    return true;
}

bool MrqDevice::saveEntry(const QString dirname, const QString &entry, const QFrqFile &in,
                          const QMap<QString, QVariant> &args) {
    return true;
}

QStringList MrqDevice::frequencyEntries(const QString dirname) const {
    return {};
}
