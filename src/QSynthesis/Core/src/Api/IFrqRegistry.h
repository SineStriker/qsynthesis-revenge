#ifndef __IFRQREGISTRY_H__
#define __IFRQREGISTRY_H__

#include <QObject>

#include "Frq/QFrqFile.h"

#include "qutacore_global.h"

class QUTACORE_API IFrqRegistry : public QObject {
    Q_OBJECT
public:
    IFrqRegistry(QObject *parent = nullptr);
    ~IFrqRegistry();

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
                           
    virtual QStringList getEntries(const QString dirname) const = 0;
};

#define IFrqRegistry_IID "QSynthesis.Plugin.SVS.FrqRegistry"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IFrqRegistry, IFrqRegistry_IID)
QT_END_NAMESPACE

#endif // __IFRQREGISTRY_H__