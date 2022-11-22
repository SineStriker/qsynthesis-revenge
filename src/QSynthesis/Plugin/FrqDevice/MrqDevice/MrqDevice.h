#ifndef MRQDEVICE_H
#define MRQDEVICE_H

#include "Api/IFrqDevice.h"

class Q_DECL_EXPORT MrqDevice : public IFrqDevice {
    Q_OBJECT
    Q_INTERFACES(IFrqDevice)
    Q_PLUGIN_METADATA(IID IFrqDevice_IID FILE "plugin.json")
public:
    explicit MrqDevice(QObject *parent = nullptr);
    ~MrqDevice();

public:
    bool loadEntry(const QString dirname,                   // Directory
                   const QString &entry,                    // Entry
                   QFrqFile *out,                           // Out
                   const QMap<QString, QVariant> &args = {} // Arguments
                   ) override;
    bool saveEntry(const QString dirname,                   // Directory
                   const QString &entry,                    // Entry
                   const QFrqFile &in,                      // In
                   const QMap<QString, QVariant> &args = {} // Arguments
                   ) override;
    QStringList frequencyEntries(const QString dirname) const override;

signals:
};

#endif // MRQDEVICE_H
