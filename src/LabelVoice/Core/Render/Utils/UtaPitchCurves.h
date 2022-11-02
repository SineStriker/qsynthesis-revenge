#ifndef UTAPITCHCURVES_H
#define UTAPITCHCURVES_H

#include "Utau/QUstPoint.h"

#include "lvcore_global.h"

#define PI 3.1415926

extern const char Base64EncodeMap[];

namespace UtaPitchCurves {

    LVCORE_API double f_x(const double &x1, const double &y1, const double &x2, const double &y2,
                          const double &x);

    LVCORE_API double f_r(const double &x1, const double &y1, const double &x2, const double &y2,
                          const double &x);

    LVCORE_API double f_j(const double &x1, const double &y1, const double &x2, const double &y2,
                          const double &x);

    LVCORE_API double f_s(const double &x1, const double &y1, const double &x2, const double &y2,
                          const double &x);

    LVCORE_API double f_type(const QUstPoint::PointType &ptype, const double &x1, const double &y1,
                             const double &x2, const double &y2, const double &x);

    LVCORE_API QList<int> convert_from_vector_point(
        const double &tempo1, const QList<QUstPoint> &curNote, const QList<double> &curVBR,
        const double &curPre, const double &curStp, const int &curLength,

        const QList<QUstPoint> &nextNote = {}, const QList<double> &nextVBR = {},
        const double &nextPre = 0, const double &nextOve = 0, const int &nextLength = 0,

        const QList<QUstPoint> &prevNote = {}, const QList<double> &prevVBR = {},
        const int &prevLength = 0);

    LVCORE_API QString encode_from_vector(const QList<int> &pitchBend);

    LVCORE_API QString encode_single_num(int n);

    LVCORE_API int find_impact(const QList<QUstPoint> &portamento, int &startIndex, int curTick,
                               const double &PositiveTempo, double NegativeTempo = 0,
                               const QList<double> &vibrato = {}, const int &length = 0);

}; // namespace UtaPitchCurves

#endif // UTAPITCHCURVES_H
