#ifndef QMIDIIMPORTER_H
#define QMIDIIMPORTER_H

#include "api/IFormatImporter.h"

class Q_DECL_EXPORT QMidiImporter : public QObject, public IFormatImporter {
    Q_OBJECT
    Q_INTERFACES(IFormatImporter)
    Q_PLUGIN_METADATA(IID IFormatImporter_IID FILE "plugin.json")
public:
    QMidiImporter();
    ~QMidiImporter();

    bool parse(const QString &filename, Callback callback, QUstFile *out) override;
};

#endif // QMIDIIMPORTER_H
