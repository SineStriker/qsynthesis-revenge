#ifndef QMIDICONVERTER_H
#define QMIDICONVERTER_H

#include "api/IUstConverter.h"

#include <QObject>

class Q_DECL_EXPORT QMidiConverter : public QObject, public IUstConverter {
    Q_OBJECT
    Q_INTERFACES(IUstConverter)
    Q_PLUGIN_METADATA(IID IUstConverter_IID FILE "plugin.json")
public:
    explicit QMidiConverter(QObject *parent = nullptr);
    ~QMidiConverter();

    bool load(const QString &filename, QUstFile *out, Callback callback) override;
    bool save(const QString &filename, const QUstFile &in) override;
};

#endif // QMIDICONVERTER_H
