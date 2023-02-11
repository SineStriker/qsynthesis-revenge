#ifndef ISVSPLUGIN_H
#define ISVSPLUGIN_H

#include <QObject>

#include "INamePlugin.h"
#include "QsSVSGlobal.h"

class QSSVS_API ISVSPlugin : public INamePlugin {
    Q_OBJECT
public:
    explicit ISVSPlugin(QObject *parent = nullptr);
    ~ISVSPlugin();
};

#endif // ISVSPLUGIN_H
