#ifndef UTAPITCHCURVES_H
#define UTAPITCHCURVES_H

#include "../QUstPoint.h"

#include "qssvs_global.h"

namespace UtaPitchCurves {

    QSSVS_API double f_x(const double &x1, const double &y1, const double &x2, const double &y2,
                         const double &x);

    QSSVS_API double f_r(const double &x1, const double &y1, const double &x2, const double &y2,
                         const double &x);

    QSSVS_API double f_j(const double &x1, const double &y1, const double &x2, const double &y2,
                         const double &x);

    QSSVS_API double f_s(const double &x1, const double &y1, const double &x2, const double &y2,
                         const double &x);

    QSSVS_API double f_type(const QUstPoint::PointType &ptype, const double &x1, const double &y1,
                            const double &x2, const double &y2, const double &x);

    QSSVS_API QList<int> convert_from_vector_point(
        const double &tempo1, const QList<QUstPoint> &curNote, const QList<double> &curVBR,
        const double &curPre, const double &curStp, const int &curLength,

        const QList<QUstPoint> &nextNote = {}, const QList<double> &nextVBR = {},
        const double &nextPre = 0, const double &nextOve = 0, const int &nextLength = 0,

        const QList<QUstPoint> &prevNote = {}, const QList<double> &prevVBR = {},
        const int &prevLength = 0);

    QSSVS_API QString encode_from_vector(const QList<int> &pitchBend);

    QSSVS_API QString encode_single_num(int n);

    QSSVS_API int find_impact(const QList<QUstPoint> &portamento, int &startIndex, int curTick,
                              const double &PositiveTempo, double NegativeTempo = 0,
                              const QList<double> &vibrato = {}, const int &length = 0);

}; // namespace UtaPitchCurves

#endif // UTAPITCHCURVES_H
