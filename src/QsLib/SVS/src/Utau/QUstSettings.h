#ifndef QUSTSETTINGS_H
#define QUSTSETTINGS_H

#include "Base/QUtaItem.h"

#include <QString>

class QSSVS_API QUstSettings : public QUtaItem {
public:
    double globalTempo;
    QString globalFlags;

    QString projectName;
    QString outputFileName;

    QString voiceDirectory;
    QString cacheDirectory;

    QString wavtoolPath;
    QString resamplerPath;

    bool isMode2;

    QUstSettings();
    ~QUstSettings();

    void reset() override;

    bool operator==(const QUstSettings &another) const;
    bool operator!=(const QUstSettings &another) const;
};

#endif // QUSTSETTINGS_H
