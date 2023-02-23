#ifndef HTTPHOST_H
#define HTTPHOST_H

#include "Api/ISynthHost.h"

DSAPI_USING_NAMESPACE

class Q_DECL_EXPORT HttpHost : public ISynthHost {
    Q_OBJECT
    Q_INTERFACES(DsApi::ISynthHost)
    Q_PLUGIN_METADATA(IID DsApi_ISynthHost_IID FILE "plugin.json")
public:
    explicit HttpHost(QObject *parent = nullptr);
    ~HttpHost();
};

#endif // HTTPHOST_H
