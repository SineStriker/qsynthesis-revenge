#ifndef __MRQREGISTRY_H__
#define __MRQREGISTRY_H__

#include "Api/IFrqRegistry.h"

class Q_DECL_EXPORT MrqRegistry : public IFrqRegistry {
    Q_OBJECT
    Q_INTERFACES(IFrqRegistry)
    Q_PLUGIN_METADATA(IID IFrqRegistry_IID FILE "plugin.json")
public:
    explicit MrqRegistry(QObject *parent = nullptr);
    ~MrqRegistry();

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
                   
    QStringList getEntries(const QString dirname) const override;

signals:
};


#endif // __MRQREGISTRY_H__