#ifndef QMIDICONVERTER_H
#define QMIDICONVERTER_H

#include "api/ISVSConverter.h"

class Q_DECL_EXPORT QMidiConverter : public QObject, public ISVSConverter {
    Q_OBJECT
    Q_INTERFACES(ISVSConverter)
    Q_PLUGIN_METADATA(IID IFormatImporter_IID FILE "plugin.json")
public:
    QMidiConverter();
    ~QMidiConverter();

    bool load(const QString &filename, QUstFile *out, Callback callback) override;
    bool save(const QString &filename, const QUstFile &in) override;
};

#endif // QMIDICONVERTER_H
