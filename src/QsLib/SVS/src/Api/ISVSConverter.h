#ifndef ISVSCONVERTER_H
#define ISVSCONVERTER_H

#include "ISVSPlugin.h"

#include <functional>

#include "Svip/QSvipFile.h"

#include "qssvs_global.h"

class QSSVS_API ISVSConverter : public ISVSPlugin {
    Q_OBJECT
public:
    ISVSConverter(QObject *parent = nullptr);
    ~ISVSConverter();

    virtual bool load(const QString &filename, QSvipFile *out,
                      const QMap<QString, QVariant> &args) = 0;

    virtual bool save(const QString &filename, const QSvipFile &in,
                      const QMap<QString, QVariant> &args) = 0;

    QString err;
};

#define ISVSConverter_IID "QSynthesis.Plugin.SVS.SVSConverter"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ISVSConverter, ISVSConverter_IID)
QT_END_NAMESPACE

#endif // ISVSCONVERTER_H
