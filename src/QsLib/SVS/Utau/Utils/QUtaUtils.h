#ifndef QUSTUTILS_H
#define QUSTUTILS_H

#include <QList>
#include <QString>

#include "../QOtoItem.h"
#include "../QUstNote.h"
#include "../QUstPoint.h"

#include "QsSVSGlobal.h"

namespace QUtaUtils {

    const char NODEF_STRING[] = "%NODEF%";
    const int NODEF_INT = INT_MIN;
    const double NODEF_DOUBLE = -1e+08;

    // Structs
    struct PBStrings {
        QString PBS;
        QString PBW;
        QString PBY;
        QString PBM;
    };

    QSSVS_API QString fromFrqName(const QString &filename);
    QSSVS_API QString toFrqName(const QString &filename);
    QSSVS_API QString mrqName();

    QSSVS_API QList<QUstPoint> DefaultEnvelope();
    QSSVS_API QList<double> DefaultVibrato();
    QSSVS_API QList<QUstPoint> DefaultPortamento();

    QSSVS_API QString PointToString(QUstPoint::PointType type);
    QSSVS_API QUstPoint::PointType StringTpoint(const QString &name);

    QSSVS_API QList<QUstPoint> StringToPortamento(const PBStrings &pbstr);
    QSSVS_API PBStrings PortamentoToString(const QList<QUstPoint> &points);

    QSSVS_API QList<QUstPoint> StringToEnvelope(const QString &str);
    QSSVS_API QString EnvelopeToString(const QList<QUstPoint> &points);

    QSSVS_API QOtoItem StringToGenon(const QString &str);
    QSSVS_API QString GenonToString(const QOtoItem &genon);

    QSSVS_API QList<double> StringsToDoubles(const QStringList &strs);
    QSSVS_API QStringList DoublesToStrings(const QList<double> &nums);

    QSSVS_API int ToneNameToToneNum(const QString &name);
    QSSVS_API QString ToneNumToToneName(int num);
    QSSVS_API QString ToneNumToToneName(int nameIndex, int octaveIndex);

    QSSVS_API double TickToTime(int tick, double tempo);
    QSSVS_API int TimeToTick(double time, double tempo);
    QSSVS_API bool isRestLyric(const QString &oLyric);

    QSSVS_API int StandardToneNum();

    QSSVS_API QString fromUSTVoiceDir(const QString &filePath, const QString &voicePath);
    QSSVS_API QString toUSTVoiceDir(const QString &filePath, const QString &voicePath);
    QSSVS_API QString fromUSTToolsDir(const QString &filePath);
    QSSVS_API QString toUSTToolsDir(const QString &filePath);

    // Default values
    enum NoteProperty {
        Intensity,
        Modulation,
        Velocity,
        Lyric,
        Flags,
        NoteNum,
        Length,
        PreUtterance,
        VoiceOverlap,
        StartPoint,
        Tempo,
        PitchBend,
        PitchType,
        PitchStart,
        Portamento,
        Vibrato,
        Envelope,
    };

    QSSVS_API double prop(double val, NoteProperty prop, double def = NODEF_DOUBLE);

    QSSVS_API QString prop(const QString &val, NoteProperty prop,
                           const QString &def = NODEF_STRING);

} // namespace QUtaUtils

#endif // QUSTUTILS_H
