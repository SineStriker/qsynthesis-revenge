#ifndef ISVSPLUGIN_H
#define ISVSPLUGIN_H

#include <QObject>

#include "INamePlugin.h"
#include "QsSVSGlobal.h"

QSAPI_BEGIN_NAMESPACE

class QSSVS_API ISVSPlugin : public INamePlugin {
    Q_OBJECT
public:
    explicit ISVSPlugin(QObject *parent = nullptr);
    ~ISVSPlugin();
};

QSAPI_END_NAMESPACE

#endif // ISVSPLUGIN_H
