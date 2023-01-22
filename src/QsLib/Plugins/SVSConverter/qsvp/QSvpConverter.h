#ifndef QSVPCONVERTER_H
#define QSVPCONVERTER_H

#include "Api/ISVSConverter.h"

class Q_DECL_EXPORT QSvpConverter : public ISVSConverter {
    Q_OBJECT
    Q_INTERFACES(ISVSConverter)
    Q_PLUGIN_METADATA(IID ISVSConverter_IID FILE "plugin.json")
public:
    explicit QSvpConverter(QObject *parent = nullptr);
    ~QSvpConverter();

    bool load(const QString &filename, QSvipModel *out,
              const QMap<QString, QVariant> &args = {}) override;

    bool save(const QString &filename, const QSvipModel &in,
              const QMap<QString, QVariant> &args = {}) override;
};

#endif // QSVPCONVERTER_H
