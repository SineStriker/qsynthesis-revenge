#ifndef USTPORTER_H
#define USTPORTER_H

#include "Api/IDspxPorter.h"

class Q_DECL_EXPORT UstPorter : public IDspxPorter {
    Q_OBJECT
    Q_INTERFACES(IDspxPorter)
    Q_PLUGIN_METADATA(IID IDspxPorter_IID FILE "plugin.json")
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
