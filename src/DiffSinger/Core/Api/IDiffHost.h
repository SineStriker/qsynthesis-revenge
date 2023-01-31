#ifndef IDIFFHOST_H
#define IDIFFHOST_H

#include <QObject>

#include "DsCoreGlobal.h"
#include "INamePlugin.h"

class DSCORE_API IDiffHost : public INamePlugin {
    Q_OBJECT
public:
    IDiffHost(QObject *parent = nullptr);
    ~IDiffHost();

public:
};

#define IDiffHost_IID "DiffSinger.Plugin.DiffHost"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IDiffHost, IDiffHost_IID)
QT_END_NAMESPACE

#endif // IDIFFHOST_H
