#ifndef UNOTE_H
#define UNOTE_H

#include <QPoint>
#include <QStringList>

/**
 *  Interpret from OpenUtau.Core.Ustx
 */

class UNote;

class UVibrato {
public:
    UVibrato();
    ~UVibrato();

    void clear();

    double length() const;
    void setLength(double value);

    double period() const;
    void setPeriod(double value);

    double depth() const;
    void setDepth(double value);

    double in() const;
    void setIn(double value);

    double out() const;
    void setOut(double value);

    double shift() const;
    void setShift(double value);

    double drift() const;
    void setDrift(double value);

    double normalizedStart() const;

private:
    double _length; // Vibrato percentage of note length.
    double _period; // Period in milliseconds.
    double _depth;  // Depth in cents (1 semitone = 100 cents).
    double _in;     // Fade-in percentage of vibrato length.
    double _out;    // Fade-out percentage of vibrato length.
    double _shift;  // Shift percentage of period length.
    double _drift;

public:
    QPointF evaluate(double nPos, double nPeriod, const UNote &note) const;
};

class PitchPoint {
public:
    enum class Shape { io, l, i, o };

    double X;
    double Y;
    Shape shape;

    PitchPoint();
    PitchPoint(double x, double y, Shape shape = Shape::io);

    bool operator<(const PitchPoint &another) const;
    bool operator==(const PitchPoint &another) const;

    static QString ShapeToString(Shape shape);
    static Shape StringToShape(const QString &str);
};

class UPitch {
public:
    UPitch();
    ~UPitch();

    void clear();

    void addPoint(const PitchPoint &p);
    void removePoint(const PitchPoint &p);

    QList<PitchPoint> data;
    bool snapFirst;
};

class UNote {
public:
    UNote();
    ~UNote();

    void clear();

public:
    int end() const;

    int position;
    int duration;
    int tone;
    QString lyric;
    UPitch pitch;
    UVibrato vibrato;

    //    bool operator<(const UNote &another) const;
    //    friend uint qHash(const UNote &note, uint seed);
};

#endif // UNOTE_H
