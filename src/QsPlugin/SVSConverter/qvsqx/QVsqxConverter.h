#ifndef QVSQXCONVERTER_H
#define QVSQXCONVERTER_H

#include "Api/ISVSConverter.h"

class Q_DECL_EXPORT QVsqxConverter : public ISVSConverter {
    Q_OBJECT
    Q_INTERFACES(ISVSConverter)
    Q_PLUGIN_METADATA(IID ISVSConverter_IID FILE "plugin.json")
public:
    explicit QVsqxConverter(QObject *parent = nullptr);
    ~QVsqxConverter();

    bool load(const QString &filename, QSvipFile *out) override;
    bool save(const QString &filename, const QSvipFile &in) override;
};

#endif // QVSQXCONVERTER_H
