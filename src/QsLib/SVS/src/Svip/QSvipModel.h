#ifndef QSVIPMODEL_H
#define QSVIPMODEL_H

#include <QJsonObject>
#include <QSharedPointer>

#include "qssvs_global.h"

/**
 *  This model is a C++ implementation of OpenSVIP.Model
 */

class QSSVS_API QSvipModel {
public:
    QSvipModel();
    ~QSvipModel();

    class QSSVS_API ParamCurve {
    public:
        int TotalPointsCount() const {
            return PointList.size();
        };

    public:
        QList<QPair<int, int>> PointList;
    };

    class QSSVS_API Params {
    public:
        ParamCurve Pitch;
        ParamCurve Volume;
        ParamCurve Breath;
        ParamCurve Gender;
        ParamCurve Strength;
    };

    class QSSVS_API Vibrato {
    public:
        double StartPercent = 0.0;
        double EndPercent = 0.0;
        bool IsAntiPhase = false;
        ParamCurve Amplitude;
        ParamCurve Frequency;
    };

    class QSSVS_API Phones {
    public:
        double HeadLengthInSecs = -1.0;
        double MidRatioOverTail = -1.0;
    };

    class QSSVS_API Note {
    public:
        int StartPos = 0;
        int Length = 0;
        int KeyNumber = 60;
        QString HeadTag;
        QString Lyric;
        QString Pronunciation;
        Phones EditedPhones;
        QSvipModel::Vibrato Vibrato;
    };

    class QSSVS_API Track {
    public:
        enum Type { Singing, Instrumental };

        Track(int type) : t(type){};

    public:
        inline int type() const {
            return t;
        }

        QString Title;
        bool Mute = false;
        bool Solo = true;
        double Volume = 0.0;
        double Pan = 0.0;

    protected:
        int t;
    };

    class QSSVS_API SingingTrack : public Track {
    public:
        SingingTrack() : Track(Singing) {
        }

    public:
        QString AISingerName;
        QString ReverbPreset;
        QList<Note> NoteList;
        Params EditedParams;
    };

    class QSSVS_API InstrumentalTrack : public Track {
    public:
        InstrumentalTrack() : Track(Instrumental) {
        }

    public:
        QString AudioFilePath;
        int Offset = 0;
    };

    class QSSVS_API TimeSignature {
    public:
        int BarIndex = 0;
        int Numerator = 0;
        int Denominator = 0;
    };

    class QSSVS_API SongTempo {
    public:
        int Position = 0;
        double BPM = 120.0;
    };

    using TrackRef = QSharedPointer<Track>;

    QString Version;
    QList<SongTempo> SongTempoList;
    QList<TimeSignature> TimeSignatureList;
    QList<TrackRef> TrackList;

public:
    QJsonObject toJsonObject() const;
    static QSvipModel fromJsonObject(const QJsonObject &obj);

    void reset();
};

#endif // QSVIPMODEL_H
