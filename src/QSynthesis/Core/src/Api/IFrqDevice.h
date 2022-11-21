#ifndef __IFRQDEVICE_H__
#define __IFRQDEVICE_H__

#include <QObject>

#include "Frq/QFrqFile.h"

#include "qutacore_global.h"

class QUTACORE_API IFrqDevice : public QObject {
    Q_OBJECT
public:
    IFrqDevice(QObject *parent = nullptr);
    ~IFrqDevice();

public:
    virtual bool loadEntry(const QString dirname,                   // Directory
                           const QString &entry,                    // Entry
                           QFrqFile *out,                           // Out
                           const QMap<QString, QVariant> &args = {} // Arguments
                           ) = 0;
    virtual bool saveEntry(const QString dirname,                   // Directory
                           const QString &entry,                    // Entry
                           const QFrqFile &in,                      // In
                           const QMap<QString, QVariant> &args = {} // Arguments
                           ) = 0;
    virtual QStringList frequencyEntries(const QString dirname) const = 0;
};

#define IFrqDevice_IID "QSynthesis.Plugin.SVS.FrqDevice"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IFrqDevice, IFrqDevice_IID)
QT_END_NAMESPACE

#endif // __IFRQDEVICE_H__
