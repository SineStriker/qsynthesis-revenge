#ifndef HTTPHOST_H
#define HTTPHOST_H

#include "Api/ISynthHost.h"

class Q_DECL_EXPORT HttpHost : public ISynthHost {
    Q_OBJECT
    Q_INTERFACES(ISynthHost)
    Q_PLUGIN_METADATA(IID ISynthHost_IID FILE "plugin.json")
public:
    explicit HttpHost(QObject *parent = nullptr);
    ~HttpHost();
};

#endif // HTTPHOST_H
