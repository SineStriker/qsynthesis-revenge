#ifndef ISYNTHHOST_H
#define ISYNTHHOST_H

#include <QObject>

#include "DsCoreGlobal.h"
#include "INamePlugin.h"

DSAPI_BEGIN_NAMESPACE

class DSCORE_API ISynthHost : public QsApi::INamePlugin {
    Q_OBJECT
public:
    explicit ISynthHost(QObject *parent = nullptr);
    ~ISynthHost();

public:
};

DSAPI_END_NAMESPACE

#define DsApi_ISynthHost_IID "DiffScope.Plugin.SynthHost"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(DsApi::ISynthHost, DsApi_ISynthHost_IID)
QT_END_NAMESPACE


#endif // ISYNTHHOST_H
