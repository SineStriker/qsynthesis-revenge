#ifndef QUSTSETTINGS_H
#define QUSTSETTINGS_H

#include <QString>

#include "qssvs_global.h"

class QSSVS_API QUstSettings {
public:
    QUstSettings();
    ~QUstSettings();

    void clear();

    double globalTempo;
    QString globalFlags;

    QString projectName;
    QString outputFileName;

    QString voiceDirectory;
    QString cacheDirectory;

    QString wavtoolPath;
    QString resamplerPath;

    bool isMode2;

    bool operator==(const QUstSettings &another) const;
    bool operator!=(const QUstSettings &another) const;
};

#endif // QUSTSETTINGS_H
