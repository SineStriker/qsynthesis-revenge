#ifndef ISYNTHHOST_H
#define ISYNTHHOST_H

#include <QObject>

#include "DsCoreGlobal.h"
#include "INamePlugin.h"

class DSCORE_API ISynthHost : public INamePlugin {
    Q_OBJECT
public:
    explicit ISynthHost(QObject *parent = nullptr);
    ~ISynthHost();

public:
};

#define ISynthHost_IID "DiffScope.Plugin.SynthHost"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ISynthHost, ISynthHost_IID)
QT_END_NAMESPACE


#endif // ISYNTHHOST_H
