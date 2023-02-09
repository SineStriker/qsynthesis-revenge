#ifndef MIDIPORTER_H
#define MIDIPORTER_H

#include "Api/IDsPorter.h"

class Q_DECL_EXPORT MidiPorter : public IDsPorter {
    Q_OBJECT
    Q_INTERFACES(IDsPorter)
    Q_PLUGIN_METADATA(IID IDsPorter_IID FILE "plugin.json")
public:
    explicit MidiPorter(QObject *parent = nullptr);
    ~MidiPorter();

public:
    QString filterString() const override;

    bool load(const QString &filename, QDspxModel *out,
              const QMap<QString, QVariant> &args = {}) override;

    bool save(const QString &filename, const QDspxModel &in,
              const QMap<QString, QVariant> &args = {}) override;
};

#endif // MIDIPORTER_H
