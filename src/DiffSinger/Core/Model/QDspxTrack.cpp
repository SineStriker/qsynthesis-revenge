#include "QDspxTrack.h"

QString QDspx::Clip::TypeToString(QDspx::Clip::Type type) {
    QString res;
    switch (type) {
        case Singing:
            res = "singing";
            break;
        case Audio:
            res = "audio";
            break;
        default:
            break;
    }
    return res;
}

QDspx::Clip::Type QDspx::Clip::StringToType(const QString &s) {
    Type res = Singing;
    if (s == "singing") {
        res = Singing;
    } else if (s == "audio") {
        res = Audio;
    }
    return res;
}
