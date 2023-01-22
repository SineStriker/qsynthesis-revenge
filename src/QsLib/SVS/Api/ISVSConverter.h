#ifndef ISVSCONVERTER_H
#define ISVSCONVERTER_H

#include <functional>

#include "ISVSPlugin.h"
#include "Svip/QSvipModel.h"

class QSSVS_API ISVSConverter : public ISVSPlugin {
    Q_OBJECT
public:
    ISVSConverter(QObject *parent = nullptr);
    ~ISVSConverter();

    QString err;

    virtual bool load(const QString &filename, QSvipModel *out,
                      const QMap<QString, QVariant> &args = {}) = 0;

    virtual bool save(const QString &filename, const QSvipModel &in,
                      const QMap<QString, QVariant> &args = {}) = 0;
};

#define ISVSConverter_IID "QSynthesis.Plugin.SVS.SVSConverter"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ISVSConverter, ISVSConverter_IID)
QT_END_NAMESPACE

#endif // ISVSCONVERTER_H
