#include "MrqRegistry.h"

MrqRegistry::MrqRegistry(QObject *parent) : IFrqRegistry(parent) {
}

MrqRegistry::~MrqRegistry() {
}

bool MrqRegistry::loadEntry(const QString dirname, const QString &entry, QFrqFile *out,
                          const QMap<QString, QVariant> &args) {
    return true;
}

bool MrqRegistry::saveEntry(const QString dirname, const QString &entry, const QFrqFile &in,
                          const QMap<QString, QVariant> &args) {
    return true;
}

QStringList MrqRegistry::getEntries(const QString dirname) const {
    return {};
}
