//
// Created by Functioner on 2020/12/9.
//
#include "UtaPitchCurves.h"

#include "Utau/QUstUtils.h"

#include <QtMath>

const char Base64EncodeMap[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

double UtaPitchCurves::f_x(const double &x1, const double &y1, const double &x2, const double &y2,
                           const double &x) {
    if (x1 == x2) {
        return y1;
    }
    return (y1 - y2) / 2 * qCos(PI * (x - x1) / (x2 - x1)) + (y1 + y2) / 2;
}


double UtaPitchCurves::f_r(const double &x1, const double &y1, const double &x2, const double &y2,
                           const double &x) {
    if (x1 == x2) {
        return y1;
    }
    return (y2 - y1) * qCos(PI / 2 / (x2 - x1) * (x - x2)) + y1;
}


double UtaPitchCurves::f_j(const double &x1, const double &y1, const double &x2, const double &y2,
                           const double &x) {
    if (x1 == x2) {
        return y1;
    }
    return (y1 - y2) * qCos(PI / 2 / (x2 - x1) * (x - x1)) + y2;
}


double UtaPitchCurves::f_s(const double &x1, const double &y1, const double &x2, const double &y2,
                           const double &x) {
    if (x1 == x2) {
        return y1;
    }
    return (y2 - y1) / (x2 - x1) * (x - x1) + y1;
}

double UtaPitchCurves::f_type(const QUstPoint::PointType &ptype, const double &x1, const double &y1,
                              const double &x2, const double &y2, const double &x) {
    int impact;

    switch (ptype) {
        case QUstPoint::linearJoin:
            impact = int(f_s(x1, y1, x2, y2, x));
            break;
        case QUstPoint::jJoin:
            impact = int(f_j(x1, y1, x2, y2, x));
            break;
        case QUstPoint::rJoin:
            impact = int(f_r(x1, y1, x2, y2, x));
            break;
        default:
            impact = int(f_x(x1, y1, x2, y2, x));
            break;
    }

    return impact;
}

QList<int> UtaPitchCurves::convert_from_vector_point(
    const double &tempo1, const QList<QUstPoint> &curNote, const QList<double> &curVBR,
    const double &curPre, const double &curStp, const int &curLength,
    const QList<QUstPoint> &nextNote, const QList<double> &nextVBR, const double &nextPre,
    const double &nextOve, const int &nextLength, const QList<QUstPoint> &prevNote,
    const QList<double> &prevVBR, const int &prevLength) {

    // Mode 2 to Mode 1 principle
    // 1. Pre-Utterance part, use the previous note tempo (actually not)
    // 2. For the rest part, use its own tempo
    // 3. Pre-Utterance may be affected by the pitch line of the preceding note
    // 4. The rest part may be affected by the pitch line of the next note

    // curNote saves its own control point, nextNote saves the control point of the nextNote,
    // prevNote saves the control point of the previous note
    // tempo stores its own tempo, tempo2 stores the previous note's tempo

    QList<int> PitchBend;
    double duration, nextStart, pbstart;
    int basePitch, prevImpact, nextImpact;
    int i, j, k; // Self, successor, precursor counter

    double tick;

    double curTempo = tempo1;
    double prevTempo = tempo1;

    // Ignore the length of the pre-utterance
    duration = double(curLength) + (-nextPre + nextOve) * curTempo / 60 * 480 / 1000;
    // The next note starts the effect
    nextStart =
        (!nextNote.empty()) ? (curLength + nextNote[0].X * curTempo / 60 * 480 / 1000) : INFINITY;

    i = 0;
    j = 0;
    k = 0;

    // Set the starting point on the previous note (- PBStart = STP + pre)
    pbstart = -(curPre + curStp) * prevTempo / 60 * 480 / 1000;
    tick = pbstart;

    while (tick < duration) {
        prevImpact = 0;
        nextImpact = 0;

        basePitch = find_impact(curNote, i, tick, curTempo, prevTempo, curVBR, curLength);

        // The part influenced by the next note
        if (tick >= nextStart) {
            if (j < nextNote.size() - 1) {
                nextImpact = find_impact(nextNote, j, tick - curLength, curTempo, curTempo, nextVBR,
                                         nextLength);
            }
            nextImpact += -nextNote[0].Y * 10;
        }

        // The part influenced by the previous note
        if (tick <= 0) {
            prevImpact = find_impact(prevNote, k, tick + prevLength, prevTempo, prevTempo, prevVBR,
                                     prevLength);
        }

        // Add the influence of the pitch line before and after the note
        PitchBend.push_back(basePitch + prevImpact + nextImpact);
        tick = tick + 5;
    }

    // Delete the redundant 0
    while (PitchBend.size() >= 7 && PitchBend.back() == 0) {
        PitchBend.pop_back();
    }

    return PitchBend;
}

QString UtaPitchCurves::encode_single_num(int n) {
    QChar x, y;
    QString result;

    // If the value is negative, the 12-bit binary is inverted
    if (n < 0) {
        n += 4096;
    }

    x = QChar(Base64EncodeMap[int(n / 64)]);
    y = QChar(Base64EncodeMap[n % 64]);

    result.append(x);
    result.append(y);

    return result;
}

int UtaPitchCurves::find_impact(const QList<QUstPoint> &portamento, int &startIndex, int curTick,
                                const double &PositiveTempo, double NegativeTempo,
                                const QList<double> &vibrato, const int &length) {

    // portamento: Mode2 Pitch curve points
    // startIndex: search from index
    // curTick: current tick
    // PositiveTempo: tempo used when ticks is positive
    // NegativeTempo: tempo used when ticks is negative (necessary when tempo changed)
    // vibrato: The vibrato sequence, which can be absent

    if (startIndex < 0) {
        return 0;
    }

    int basePitch;
    int &i = startIndex; // Self counter
    double x1, y1, x2, y2;
    QUstPoint::PointType ptype;
    bool tooLeft;

    basePitch = 0;
    // Search portatmento
    if (!portamento.empty()) {
        tooLeft = false;

        while (i < portamento.size() - 1) {
            // Last point
            x1 = portamento[i].X * ((portamento[i].X < 0) ? NegativeTempo : PositiveTempo) / 60 *
                 480 / 1000;

            if (curTick < x1) {
                if (i > 0) {
                    i--; // startIndex is incorrect, need to search forward
                    continue;
                } else if (i == 0) {
                    tooLeft = true;
                    break;
                }
            }

            y1 = portamento[i].Y * 10;

            // Next point
            x2 = portamento[i + 1].X * ((portamento[i + 1].X < 0) ? NegativeTempo : PositiveTempo) /
                 60 * 480 / 1000;

            y2 = portamento[i + 1].Y * 10;

            ptype = portamento[i + 1].P;

            if (curTick > x2) {
                i++;
                continue;
            }
            basePitch = f_type(ptype, x1, y1, x2, y2, curTick);
            break;
        }
        if (tooLeft) {
            basePitch = int(portamento[0].Y * 10); // haven't reached the first point yet
        }
    }

    // Search vibrato
    if (vibrato.size() >= 8 && length > 0) {
        double proportion = vibrato[0];
        double period = vibrato[1];
        double amplitude = vibrato[2];
        double easeIn = vibrato[3];
        double easeOut = vibrato[4];
        double phase = vibrato[5];
        double offset = vibrato[6];

        proportion /= 100.0;

        double tick_time = period * PositiveTempo / 60 * 480 / 1000;
        double tick_length = proportion * length;
        double tick_start = (1 - proportion) * length; // ticks time relative to start

        double ratio;
        double x, k, p, y;

        k = 1 / tick_time * 2 * PI; // Circular frequency
        p = phase / 100.0 * 2 * PI; // Initial phase

        easeIn = easeIn / 100.0 * tick_length;         // Fade in time
        easeOut = (1 - easeOut / 100.0) * tick_length; // Fade out time
        offset /= 100.0;

        x = curTick - tick_start;       // tick x
        y = amplitude * sin(k * x - p); // original vibrato

        if (x > 0 && x < tick_length) {
            ratio = 1;
            // Add offset
            y += offset * amplitude;
            // Calculate envelope
            if (x < easeIn) {
                ratio *= x / easeIn;
            }
            if (x > easeOut) {
                ratio *= 1 - (x - easeOut) / (tick_length - easeOut);
            }
            // Add envelope
            y = ratio * y;
            // Add influence
            basePitch += y;
        }
    }

    return basePitch;
}

QString UtaPitchCurves::encode_from_vector(const QList<int> &pitchBend) {
    int pos = 0;
    int count = 0;
    int curInt;
    int prevInt = INT32_MIN;
    QString result;

    while (pos < pitchBend.size()) {
        pos++;
        curInt = (pitchBend[pos - 1] == QUstUtils::NODEF_INT) ? 0 : pitchBend[pos - 1];

        if (curInt == prevInt) {
            ++count;
            // Final process
            if (pos == pitchBend.size()) {
                if (count >= 2) {
                    result += "#" + QString::number(count) + "#";
                } else {
                    result += encode_single_num(prevInt);
                }
            }
        } else {
            if (count != 0) {
                // Use n-1 to replace the rest when appear repeatedly
                if (count >= 2) {
                    result += "#" + QString::number(count) + "#";
                } else {
                    result += encode_single_num(prevInt);
                }
                count = 0;
            }
            result += encode_single_num(curInt);
        }

        prevInt = curInt;
    }

    return result;
}
