#ifndef QUSTNOTE_H
#define QUSTNOTE_H

#include <QJsonObject>
#include <QStringList>

#include "QUstPoint.h"

class QSSVS_API QUstNote : public QUtaItem {
public:
    QUstNote();
    QUstNote(int noteNum, int length);
    QUstNote(int noteNum, int length, const QString &lrc);

public:
    // String
    QString lyric, flags;

    // Position
    int noteNum;
    int length;

    // Doubles
    double intensity, modulation, velocity;
    double preUttr, overlap, stp;
    double tempo;

    // Envelope
    QList<QUstPoint> envelope;

    // Mode 2
    QList<QUstPoint> Mode2Pitch;
    QList<double> vibrato;

    // Mode 1
    double pbstart;
    QList<double> pitches;
    QString pbtype;

    // Labels
    QString label, direct, patch, region, regionEnd;

    // Customs
    QList<QPair<QString, QString>> customData;

public:
    void reset() override;

    QJsonObject toJson() const;
    static QUstNote fromJson(const QJsonObject &obj);

    bool operator==(const QUstNote &note) const;

    QSSVS_API friend QDebug operator<<(QDebug debug, const QUstNote &note);
};

#endif // QUSTNOTE_H
