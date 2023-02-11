#ifndef IDIFFHOST_H
#define IDIFFHOST_H

#include <QObject>

#include "DsCoreGlobal.h"
#include "INamePlugin.h"

class DSCORE_API IDsEngine : public INamePlugin {
    Q_OBJECT
public:
    IDsEngine(QObject *parent = nullptr);
    ~IDsEngine();

public:
};

#define IDsEngine_IID "DiffScope.Plugin.DsEngine"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IDsEngine, IDsEngine_IID)
QT_END_NAMESPACE

#endif // IDIFFHOST_H
