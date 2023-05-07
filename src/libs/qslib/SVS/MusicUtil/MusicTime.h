#ifndef CHORUSKIT_MUSICTIME_H
#define CHORUSKIT_MUSICTIME_H

#include <QDebug>
#include <QSharedData>

class MusicTimeData;

class MusicTimeSignature {
public:
    MusicTimeSignature(int numerator = 4, int denominator = 4) : m_numerator(numerator), m_denominator(denominator){};

    bool isValid() const {
        if (m_numerator <= 0)
            return false;
        if (m_denominator != 2 && m_denominator != 4 && m_denominator != 8 && m_denominator != 16)
            return false;
        return true;
    }

    inline int ticksPerBar(int resolution) const {
        return resolution * m_numerator * 4 / m_denominator;
    }

    inline int ticksPerBeat(int resolution) const {
        return resolution * 4 / m_denominator;
    }

    inline int numerator() const {
        return m_numerator;
    }

    inline int denominator() const {
        return m_denominator;
    }

    inline QString toString() const {
        return QString("%1/%2").arg(QString::number(m_numerator), QString::number(m_denominator));
    }

    inline bool operator==(const MusicTimeSignature &t) const {
        return m_numerator == t.m_numerator && m_denominator == t.m_denominator;
    }

    inline bool operator!=(const MusicTimeSignature &t) const {
        return !(*this == t);
    }

    friend inline QDebug &operator<<(QDebug &debug, const MusicTimeSignature &t) {
        return debug << t.toString();
    }

private:
    int m_numerator;
    int m_denominator;
};

class MusicTime {
public:
    MusicTime(int resolution = 480);
    ~MusicTime();

public:
    int resolution() const;
    inline int tickPerQuarter() const {
        return resolution();
    }

    void addTempo(int tick, double tempo);
    void addTempos(const QMap<int, double> &tempos);
    bool removeTempo(int tick);
    void removeTempos(const QList<int> &ticks);
    bool hasTempo(int tick);
    const QMap<int, double> &tempos() const;

    void addTimeSignature(int bar, const MusicTimeSignature &timeSig);
    void addTimeSignatures(const QMap<int, MusicTimeSignature> &timeSigs);
    bool removeTimeSignature(int bar);
    void removeTimeSignatures(const QList<int> &bars);
    bool hasTimeSignature(int bar);
    const QMap<int, MusicTimeSignature> &timeSignatures() const;

private:
    QSharedDataPointer<MusicTimeData> d;
};

#endif // CHORUSKIT_MUSICTIME_H
