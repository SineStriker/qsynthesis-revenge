#ifndef UPROJECT_H
#define UPROJECT_H

#include "UExpression.h"
#include "UPart.h"
#include "UTrack.h"

#include <QMap>
#include <QVersionNumber>

class UProject {
public:
    UProject();
    ~UProject();

    //    struct Version {
    //        int major;
    //        int minor;
    //        int build;
    //        int revision;
    //        Version() : major(0), minor(0), build(-1), revision(-1){};
    //        Version(int major, int minor, int build, int revision)
    //            : major(major), minor(minor), build(build), revision(revision){};
    //    };

    QString name;
    QString comment;
    QString outputDir;
    QString cacheDir;
    QString ustxVersion;

    double bpm;
    int beatPerBar;
    int beatUnit;
    int resolution;

    QMap<QString, UExpressionDescriptor> expressions;
    QList<UTrack> tracks;
    QList<UVoicePart> voiceParts;
    QList<UWavePart> waveParts;

public:
    bool load(const QString &filename);
    bool save(const QString &filename);

    void reset();
};

#endif // UPROJECT_H
