#ifndef USTPORTER_H
#define USTPORTER_H

#include "Api/IDsPorter.h"

class Q_DECL_EXPORT UstPorter : public IDsPorter {
    Q_OBJECT
    Q_INTERFACES(IDsPorter)
    Q_PLUGIN_METADATA(IID IDsPorter_IID FILE "plugin.json")
public:
    explicit UstPorter(QObject *parent = nullptr);
    ~UstPorter();

public:
    SupportedPorts ports() const override;

    SupportedFormat format() const override;

    bool load(const QString &filename, QDspxModel *out, QObject *parent = nullptr) override;

    bool save(const QString &filename, const QDspxModel &in, QObject *parent = nullptr) override;
};

#endif // USTPORTER_H
