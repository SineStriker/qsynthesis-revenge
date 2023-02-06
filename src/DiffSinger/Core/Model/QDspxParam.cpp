#include "QDspxParam.h"

QString QDspx::ParamCurve::TypeToString(Type type) {
    QString res;
    switch (type) {
        case Anchor:
            res = "anchor";
            break;
        case Free:
            res = "free";
            break;
        default:
            break;
    }
    return res;
}

QDspx::ParamCurve::Type QDspx::ParamCurve::StringToType(const QString &s) {
    Type res = Anchor;
    if (s == "anchor") {
        res = Anchor;
    } else if (s == "free") {
        res = Free;
    }
    return res;
}

// namespace QDspx