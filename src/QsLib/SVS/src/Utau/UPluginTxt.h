#ifndef UPLUGINTXT_H
#define UPLUGINTXT_H

#include <QStringList>

#include "qssvs_global.h"
#include "qsutils_macros.h"

class QSSVS_API UPluginTxt {
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
    bool load(const QString &filename);
    bool save(const QString &filename);

    void reset();
};

#endif // UPLUGINTXT_H
