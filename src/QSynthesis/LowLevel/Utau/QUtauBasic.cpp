#include "QUtauBasic.h"
using namespace Utau;

#include <QDir>

double prop(double val, Utau::NoteProperty prop, double def) {
    double res = NODEF_DOUBLE;
    switch (prop) {
    case Intensity:
        res = (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_INTENSITY : def) : val;
        break;
    case Modulation:
        res =
            (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_MODULATION : def) : val;
        break;
    case Velocity:
        res = (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_VELOCITY : def) : val;
        break;
    case PreUtterance:
        res = (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_PRE_UTTERANCE : def)
                                    : val;
        break;
    case VoiceOverlap:
        res = (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_VOICE_OVERLAP : def)
                                    : val;
        break;
    case StartPoint:
        res =
            (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_START_POINT : def) : val;
        break;
    case Tempo:
        res = (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_TEMPO : def) : val;
        break;
    default:
        break;
    }
    return res;
}

QString prop(const QString &val, Utau::NoteProperty prop, const QString &def) {
    QString res = NODEF_STRING;
    switch (prop) {
    case Flags:
        res = (val == NODEF_STRING) ? ((def == NODEF_STRING) ? DEFAULT_VALUE_FLAGS : def) : val;
        break;
    default:
        break;
    }
    return res;
}
