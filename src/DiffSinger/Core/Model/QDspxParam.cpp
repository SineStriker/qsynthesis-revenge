#include "QDspxParam.h"

QDspx::ParamCurveRef QASJsonType<QDspx::ParamCurveRef>::fromObject(const QJsonObject &obj, bool *ok) {
    auto it = obj.find("type");
    if (it == obj.end()) {
        return {};
    }

    bool ok2;
    auto type = QASJsonType<QDspx::ParamCurve::Type>::fromValue(it.value(), &ok2);
    if (!ok2) {
        return {};
    }

    switch (type) {
        case QDspx::ParamCurve::Anchor:
            return QDspx::ParamAnchorRef::create(QASJsonType<QDspx::ParamAnchor>::fromObject(obj, ok));
        case QDspx::ParamCurve::Free:
            return QDspx::ParamFreeRef::create(QASJsonType<QDspx::ParamFree>::fromObject(obj, ok));
        default:
            break;
    }

    return {};
}

QJsonObject QASJsonType<QDspx::ParamCurveRef>::toObject(const QDspx::ParamCurveRef &cls) {
    if (cls.isNull()) {
        return {};
    }
    return cls->type == QDspx::ParamCurve::Anchor
           ? QASJsonType<QDspx::ParamAnchor>::toObject(*cls.dynamicCast<QDspx::ParamAnchor>())
           : QASJsonType<QDspx::ParamFree>::toObject(*cls.dynamicCast<QDspx::ParamFree>());
}