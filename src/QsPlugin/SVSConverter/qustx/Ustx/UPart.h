#ifndef UPART_H
#define UPART_H

#include <QObject>
#include <QStringList>

#include "UCurve.h"
#include "UNote.h"

/**
 *  Interpret from OpenUtau.Core.Ustx
 */

class UPart {
public:
    UPart();
    ~UPart();
    virtual void clear();

    virtual QString displayName() const;
    virtual int duration() const;

    inline int endTick() const {
        return position + duration();
    }

    QString name;
    QString comment;
    int trackNo;
    int position;
};

class UVoicePart : public UPart {
public:
    UVoicePart();
    ~UVoicePart();

    void clear() override;

    QString displayName() const override;

    QList<UNote> notes;
    QList<UCurve> curves;
};

class UWavePart : public UPart {
public:
    UWavePart();
    ~UWavePart();

    void clear() override;

    QString filePath() const;
    void setFilePath(const QString &path);

    QString relativePath;
    double fileDurationMs;
    double skipMs;
    double TrimMs;

    QString displayName() const override;
    int duration() const override;

    QList<double> peaks;
    QList<double> samples;
    int channels;
    int fileDurTick;

private:
    QString _filePath;
    bool _missing;
    qint64 _duration;
};

#endif // UPART_H
