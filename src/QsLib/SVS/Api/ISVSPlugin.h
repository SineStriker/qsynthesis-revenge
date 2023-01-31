#ifndef ISVSPLUGIN_H
#define ISVSPLUGIN_H

#include <QLocale>
#include <QObject>

#include "INamePlugin.h"
#include "QsSVSGlobal.h"

class QSSVS_API ISVSPlugin : public INamePlugin {
    Q_OBJECT
public:
    ISVSPlugin(QObject *parent = nullptr);
    ~ISVSPlugin();
};

#endif // ISVSPLUGIN_H
