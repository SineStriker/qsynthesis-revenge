#ifndef QUSTCONVERTER_H
#define QUSTCONVERTER_H

#include "Api/ISVSConverter.h"

class Q_DECL_EXPORT QUstConverter : public ISVSConverter {
    Q_OBJECT
    Q_INTERFACES(ISVSConverter)
    Q_PLUGIN_METADATA(IID ISVSConverter_IID FILE "plugin.json")
public:
    explicit QUstConverter(QObject *parent = nullptr);
    ~QUstConverter();

    bool load(const QString &filename, QSvipFile *out,
              const QMap<QString, QVariant> &args) override;

    bool save(const QString &filename, const QSvipFile &in,
              const QMap<QString, QVariant> &args) override;
};

#endif // QUSTCONVERTER_H
