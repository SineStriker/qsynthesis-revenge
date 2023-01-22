#ifndef SIMPLEHOST_H
#define SIMPLEHOST_H

#include "Api/IDiffHost.h"

class Q_DECL_EXPORT SimpleHost : public IDiffHost {
    Q_OBJECT
    Q_INTERFACES(IDiffHost)
    Q_PLUGIN_METADATA(IID IDiffHost_IID FILE "plugin.json")
public:
    explicit SimpleHost(QObject *parent = nullptr);
    ~SimpleHost();
};

#endif // SIMPLEHOST_H
