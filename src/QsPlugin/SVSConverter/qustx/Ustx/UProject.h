#ifndef UPROJECT_H
#define UPROJECT_H

#include "UExpression.h"
#include "UPart.h"
#include "UTrack.h"

#include <QMap>

/**
 *  Interpret from OpenUtau.Core.Ustx
 */

class UProject {
public:
    UProject();
    ~UProject();

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

    int MillisecondToTick(double ms) const;
    double TickToMillisecond(double tick) const;
};

#endif // UPROJECT_H
