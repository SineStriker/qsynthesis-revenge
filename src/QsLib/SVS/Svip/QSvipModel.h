#ifndef QSVIPMODEL_H
#define QSVIPMODEL_H

#include <QJsonObject>
#include <QSharedPointer>

#include "QsSVSGlobal.h"

/**
 *  This model is a C++ implementation of OpenSVIP.Model
 */

class QSSVS_API QSvipModel {
public:
    QSvipModel();
    ~QSvipModel();

    class QSSVS_API ParamCurve {
    public:
        inline int TotalPointsCount() const {
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
        Vibrato() : Vibrato(0, 0) {
        }
        Vibrato(double start, double end)
            : StartPercent(start), EndPercent(end), IsAntiPhase(false) {
        }

        double StartPercent;
        double EndPercent;
        bool IsAntiPhase;
        ParamCurve Amplitude;
        ParamCurve Frequency;
    };

    class QSSVS_API Phones {
    public:
        Phones() : Phones(-1, -1) {
        }
        Phones(double head, double mid) : HeadLengthInSecs(head), MidRatioOverTail(mid) {
        }

        double HeadLengthInSecs;
        double MidRatioOverTail;
    };

    class QSSVS_API Note {
    public:
        Note() : Note(0, 0, 60) {
        }
        Note(int start, int len, int key) : StartPos(start), Length(len), KeyNumber(key) {
        }

        int StartPos;
        int Length;
        int KeyNumber;
        QString HeadTag;
        QString Lyric;
        QString Pronunciation;
        QSharedPointer<Phones> EditedPhones;
        QSharedPointer<class Vibrato> Vibrato;
    };

    class QSSVS_API Track {
    public:
        enum Type { Singing, Instrumental };

        Track(int type) : t(type), Mute(false), Solo(false), Volume(0), Pan(0){};
        virtual ~Track() {
        }

    public:
        inline int type() const {
            return t;
        }

        QString Title;
        bool Mute;
        bool Solo;
        double Volume;
        double Pan;

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
        TimeSignature() : TimeSignature(0, 0, 0) {
        }
        TimeSignature(int idx, int num, int den) : BarIndex(idx), Numerator(num), Denominator(den) {
        }

        int BarIndex;
        int Numerator;
        int Denominator;
    };

    class QSSVS_API SongTempo {
    public:
        SongTempo() : SongTempo(0, 120) {
        }
        SongTempo(int pos, double bpm) : Position(pos), BPM(bpm) {
        }

        int Position;
        double BPM;
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
