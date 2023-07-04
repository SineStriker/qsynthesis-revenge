#ifndef QUTAPLUGINTXT_H
#define QUTAPLUGINTXT_H

#include "Base/QUtaFile.h"

class QSSVS_API QUtaPluginTxt : public QUtaFile {
    Q_CHARSET
public:
    QUtaPluginTxt();
    ~QUtaPluginTxt();

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

#endif // QUTAPLUGINTXT_H
