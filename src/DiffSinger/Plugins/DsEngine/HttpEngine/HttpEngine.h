#ifndef HTTPENGINE_H
#define HTTPENGINE_H

#include "Api/IDsEngine.h"

class Q_DECL_EXPORT HttpEngine : public IDsEngine {
    Q_OBJECT
    Q_INTERFACES(IDsEngine)
    Q_PLUGIN_METADATA(IID IDsEngine_IID FILE "plugin.json")
public:
    explicit HttpEngine(QObject *parent = nullptr);
    ~HttpEngine();
};

#endif // HTTPENGINE_H
