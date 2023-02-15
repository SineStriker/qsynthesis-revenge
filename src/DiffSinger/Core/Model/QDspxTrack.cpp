#include "QDspxTrack.h"

QDspx::ClipRef QASJsonType<QDspx::ClipRef>::fromObject(const QJsonObject &obj, bool *ok) {
    auto it = obj.find("type");
    if (it == obj.end()) {
        return {};
    }

    bool ok2;
    auto type = QASJsonType<QDspx::Clip::Type>::fromValue(it.value(), &ok2);
    if (!ok2) {
        return {};
    }

    if (type == QDspx::Clip::Singing) {
        return QDspx::SingingClipRef::create(QASJsonType<QDspx::SingingClip>::fromObject(obj, ok));
    } else if (type == QDspx::Clip::Audio) {
        return QDspx::AudioClipRef::create(QASJsonType<QDspx::AudioClip>::fromObject(obj, ok));
    }

    return {};
}

QJsonObject QASJsonType<QDspx::ClipRef>::toObject(const QDspx::ClipRef &cls) {
    if (cls.isNull()) {
        return {};
    }
    return cls->type == QDspx::Clip::Singing
               ? QASJsonType<QDspx::SingingClip>::toObject(*cls.dynamicCast<QDspx::SingingClip>())
               : QASJsonType<QDspx::AudioClip>::toObject(*cls.dynamicCast<QDspx::AudioClip>());
}