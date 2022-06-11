#ifndef SECTIONSETTINGS_H
#define SECTIONSETTINGS_H

#include <QDebug>
#include <QString>

class SectionSettings {
public:
    SectionSettings();
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

    bool operator==(const SectionSettings &another) const;
    bool operator!=(const SectionSettings &another) const;
};

#endif // SECTIONSETTINGS_H
