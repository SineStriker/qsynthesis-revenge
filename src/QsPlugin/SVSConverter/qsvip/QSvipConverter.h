#ifndef QSVIPCONVERTER_H
#define QSVIPCONVERTER_H

#include "Api/ISVSConverter.h"

class Q_DECL_EXPORT QSvipConverter : public ISVSConverter {
    Q_OBJECT
    Q_INTERFACES(ISVSConverter)
    Q_PLUGIN_METADATA(IID ISVSConverter_IID FILE "plugin.json")
public:
    explicit QSvipConverter(QObject *parent = nullptr);
    ~QSvipConverter();

    bool load(const QString &filename, QSvipFile *out) override;
    bool save(const QString &filename, const QSvipFile &in) override;
};

#endif // QSVIPCONVERTER_H
