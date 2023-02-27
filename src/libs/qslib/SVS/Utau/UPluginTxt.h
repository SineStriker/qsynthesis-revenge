#ifndef UPLUGINTXT_H
#define UPLUGINTXT_H

#include "Base/QUtaFile.h"

class QSSVS_API UPluginTxt : public QUtaFile {
    Q_CHARSET
public:
    UPluginTxt();
    ~UPluginTxt();

    bool useShell;
    bool allNote;

    QString charset;
    QString name;
    QString ustVersion;
    QString execute;

    QStringList custom; // Other information which can't be edited

public:
    bool load(const QString &filename) override;
    bool save(const QString &filename) override;

    void reset() override;
    bool isEmpty() const override;
};

#endif // UPLUGINTXT_H
