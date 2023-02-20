#ifndef MIDIPORTER_H
#define MIDIPORTER_H

#include "Api/IDspxPorter.h"
#include "CDecorateDir.h"

DSAPI_USING_NAMESPACE

class Q_DECL_EXPORT MidiPorter : public IDspxPorter {
    Q_OBJECT
    Q_INTERFACES(DsApi::IDspxPorter)
    Q_PLUGIN_METADATA(IID DsApi_IDspxPorter_IID FILE "plugin.json")
public:
    explicit MidiPorter(QObject *parent = nullptr);
    ~MidiPorter();

public:
    SupportedFormat format() const override;

    bool load(const QString &filename, QDspxModel *out, QObject *parent = nullptr) override;

    bool save(const QString &filename, const QDspxModel &in, QObject *parent = nullptr) override;

protected:
    CDecorateDir dd;
};

#endif // MIDIPORTER_H
