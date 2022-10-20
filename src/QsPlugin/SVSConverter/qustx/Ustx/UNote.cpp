#include "UNote.h"

#include <QtMath>

#include <algorithm>

UVibrato::UVibrato() {
    clear();
}

UVibrato::~UVibrato() {
}

void UVibrato::clear() {
    _length = 0;
    _depth = 0;
    _in = 0;
    _out = 0;
    _shift = 0;
    _drift = 0;
}

double UVibrato::length() const {
    return _length;
}

void UVibrato::setLength(double value) {
    _length = qMax(0.0, qMin(100.0, value));
}

double UVibrato::period() const {
    return _period;
}

void UVibrato::setPeriod(double value) {
    _period = qMax(5.0, qMin(500.0, value));
}

double UVibrato::depth() const {
    return _depth;
}

void UVibrato::setDepth(double value) {
    _depth = qMax(5.0, qMin(200.0, value));
}

double UVibrato::in() const {
    return _in;
}

void UVibrato::setIn(double value) {
    _in = qMax(0.0, qMin(100.0, value));
    _out = qMin(_out, 100.0 - _in);
}

double UVibrato::out() const {
    return _out;
}

void UVibrato::setOut(double value) {
    _out = qMax(0.0, qMin(100.0, value));
    _in = qMin(_in, 100.0 - _out);
}

double UVibrato::shift() const {
    return _shift;
}

void UVibrato::setShift(double value) {
    _shift = qMax(0.0, qMin(100.0, value));
}

double UVibrato::drift() const {
    return _drift;
}

void UVibrato::setDrift(double value) {
    _drift = qMax(-100.0, qMin(100.0, value));
}

double UVibrato::normalizedStart() const {
    return 1.0 - _length / 100.0;
}

QPoint UVibrato::evaluate(double nPos, double nPeriod, const UNote &note) const {
    double nStart = normalizedStart();
    double nIn = _length / 100.0 * _in / 100.0;
    double nInPos = nStart + nIn;
    double nOut = _length / 100.0 * _out / 100.0;
    double nOutPos = 1.0 - nOut;
    double t = (nPos - nStart) / nPeriod + _shift / 100.0;
    double y = qSin(2 * M_PI * t) * _depth;
    if (nPos < nStart) {
        y = 0;
    } else if (nPos < nInPos) {
        y *= (nPos - nStart) / nIn;
    } else if (nPos > nOutPos) {
        y *= (1.0 - nPos) / nOut;
    }
    return QPoint(note.position + note.duration * nPos, note.tone + y / 100.0);
}

UPitch::UPitch() {
    clear();
}

UPitch::~UPitch() {
}

void UPitch::clear() {
    data.clear();
    snapFirst = false;
}

void UPitch::addPoint(const PitchPoint &p) {
    data.append(p);
    std::sort(data.begin(), data.end());
}

void UPitch::removePoint(const PitchPoint &p) {
    data.removeOne(p);
}

UNote::UNote() {
    clear();
}

UNote::~UNote() {
}

void UNote::clear() {
    position = 0;
    duration = 0;
    tone = 0;
    lyric = "a";
    pitch.clear();
    vibrato.clear();
}

int UNote::end() const {
    return position + duration;
}

// bool UNote::operator<(const UNote &another) const {
//     if (position < another.position) {
//         return true;
//     }
//     return qHash(*this, 0) < qHash(another, 0);
// }

// uint qHash(const UNote &note, uint seed) {
// }

PitchPoint::PitchPoint() : X(0), Y(0), shape(Shape::io) {
}

PitchPoint::PitchPoint(double x, double y, PitchPoint::Shape shape) : X(x), Y(y), shape(shape) {
}

bool PitchPoint::operator<(const PitchPoint &another) const {
    return X < another.X;
}

bool PitchPoint::operator==(const PitchPoint &another) const {
    return X == another.X && Y == another.Y && shape == another.shape;
}
