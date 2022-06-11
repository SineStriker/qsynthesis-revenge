#ifndef QLINKNOTE_H
#define QLINKNOTE_H

#include <QJsonArray>
#include <QJsonObject>
#include <QList>

#include "QControlPoint.h"
#include "QUtauStd.h"

class Utau::Note {
public:
    Note();
    Note(int noteNum, int length);
    Note(int noteNum, int length, const QString &lrc);

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
    QList<QControlPoint> envelope;

    // Mode 2
    QList<QControlPoint> Mode2Pitch;
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
    void clear();

    QJsonObject toJson() const;
    static Note fromJson(const QJsonObject &obj);

    bool operator==(const Note &note) const;

    friend QDebug operator<<(QDebug debug, const Note &oNote);
};

typedef Utau::Note QLinkNote;

Q_DECLARE_METATYPE(QLinkNote)

#endif // QLINKNOTE_H
