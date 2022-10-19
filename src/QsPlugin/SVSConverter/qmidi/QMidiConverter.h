#ifndef QMIDICONVERTER_H
#define QMIDICONVERTER_H

#include "Api/ISVSConverter.h"

class Q_DECL_EXPORT QMidiConverter : public ISVSConverter {
    Q_OBJECT
    Q_INTERFACES(ISVSConverter)
    Q_PLUGIN_METADATA(IID ISVSConverter_IID FILE "plugin.json")
public:
    explicit QMidiConverter(QObject *parent = nullptr);
    ~QMidiConverter();

    bool load(const QString &filename, QSvipFile *out,
              const QMap<QString, QVariant> &args) override;

    bool save(const QString &filename, const QSvipFile &in,
              const QMap<QString, QVariant> &args) override;
};

#endif // QMIDICONVERTER_H
