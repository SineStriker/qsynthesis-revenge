#ifndef MUSICTIMELINE_H
#define MUSICTIMELINE_H

#include <QObject>
#include <QSharedPointer>
#include <QVariant>

#include "MusicTime.h"

namespace QsApi {

    class MusicTimeline;

    class QSSVS_API MusicTimeSignature {
    public:
        Q_DECL_CONSTEXPR MusicTimeSignature();
        Q_DECL_CONSTEXPR MusicTimeSignature(int numerator, int denominator);

        Q_DECL_CONSTEXPR inline int numerator() const;
        Q_DECL_CONSTEXPR inline int denominator() const;
        Q_DECL_RELAXED_CONSTEXPR inline void setNumerator(int numerator);
        Q_DECL_RELAXED_CONSTEXPR inline void setDenominator(int denominator);

        Q_DECL_CONSTEXPR bool isValid() const;

        Q_DECL_CONSTEXPR int ticksPerBar(int resolution) const;
        Q_DECL_CONSTEXPR int ticksPerBeat(int resolution) const;

        QString toString() const;

        Q_DECL_CONSTEXPR inline bool operator==(const MusicTimeSignature &t) const;
        Q_DECL_CONSTEXPR inline bool operator!=(const MusicTimeSignature &t) const;

        friend QSSVS_API QDebug operator<<(QDebug debug, const MusicTimeSignature &t);

    private:
        int num;
        int den;
    };

    Q_DECL_CONSTEXPR int MusicTimeSignature::numerator() const {
        return num;
    }

    Q_DECL_CONSTEXPR int MusicTimeSignature::denominator() const {
        return den;
    }

    Q_DECL_RELAXED_CONSTEXPR void MusicTimeSignature::setNumerator(int numerator) {
        num = numerator;
    }

    Q_DECL_RELAXED_CONSTEXPR void MusicTimeSignature::setDenominator(int denominator) {
        den = denominator;
    }

    Q_DECL_CONSTEXPR bool MusicTimeSignature::operator==(const MusicTimeSignature &t) const {
        return num == t.num && den == t.den;
    }

    Q_DECL_CONSTEXPR bool MusicTimeSignature::operator!=(const MusicTimeSignature &t) const {
        return num != t.num || den != t.den;
    }

    QSSVS_API QDataStream &operator<<(QDataStream &out, const MusicTimeSignature &ts);
    QSSVS_API QDataStream &operator>>(QDataStream &in, MusicTimeSignature &ts);

    class MusicTimelinePrivate;

    class QSSVS_API MusicTimeline : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(MusicTimeline)
    public:
        explicit MusicTimeline(QObject *parent = nullptr);
        ~MusicTimeline();

    public:
        inline void addTimeSignature(int bar, const MusicTimeSignature &timeSignature);
        inline void removeTimeSignature(int bar);
        void addTimeSignatures(const QList<QPair<int, MusicTimeSignature>> &timeSignatureList);
        void removeTimeSignatures(const QList<int> &bars);
        QList<QPair<int, MusicTimeSignature>> timeSignatures() const;
        QMap<int, MusicTimeSignature> timeSignatureMap() const;
        QList<int> timeSignatureBars() const;
        MusicTimeSignature timeSignature(int bar) const;
        int nearestTimeSignatureBar(int bar) const;

        inline void addTempo(int tick, double tempo);
        inline void removeTempo(int tick);
        void addTempos(const QList<QPair<int, double>> &tempos);
        void removeTempos(const QList<int> &ticks);
        QList<QPair<int, double>> tempos() const;
        QMap<int, double> tempoMap() const;
        QList<int> tempoTicks() const;
        double tempo(int tick) const;
        int nearestTempoTick(int tick) const;

    public:
        MusicTime tickToTime(int totalTick) const;
        double tickToMsec(int totalTick) const;

        inline int timeToTick(const MusicTime &time) const;
        int timeToTick(int measure, int beat, int tick) const;
        int stringToTick(const QString &str) const;
        int msecToTick(double msec) const;

        inline PersistentMusicTime create(const MusicTime &time) const;
        PersistentMusicTime create(int measure, int beat, int tick) const;
        PersistentMusicTime create(const QString &str) const;
        PersistentMusicTime create(double msec) const;

    signals:
        void timeSignatureChanged();
        void tempoChanged();
        void changed();

    protected:
        MusicTimeline(MusicTimelinePrivate &d, QObject *parent = nullptr);

        QScopedPointer<MusicTimelinePrivate> d_ptr;

        friend class PersistentMusicTime;
    };

    inline void MusicTimeline::addTimeSignature(int bar,
                                                const QsApi::MusicTimeSignature &timeSignature) {
        addTimeSignatures({{bar, timeSignature}});
    }

    inline void MusicTimeline::removeTimeSignature(int bar) {
        removeTimeSignatures({bar});
    }

    inline void MusicTimeline::addTempo(int tick, double tempo) {
        addTempos({{tick, tempo}});
    }

    inline void MusicTimeline::removeTempo(int tick) {
        removeTempos({tick});
    }

    int MusicTimeline::timeToTick(const MusicTime &time) const {
        return timeToTick(time.measure(), time.beat(), time.tick());
    }

    PersistentMusicTime MusicTimeline::create(const MusicTime &time) const {
        return create(time.measure(), time.beat(), time.tick());
    }

} // namespace QsApi

Q_DECLARE_METATYPE(QsApi::MusicTimeSignature)

#endif // MUSICTIMELINE_H
