#include "QDspxParam.h"

QAS::JsonStream &QDspx::operator>>(QAS::JsonStream &_stream, QDspx::ParamCurveRef &_var) {
    QJsonObject _obj;
    if (!QAS::JsonStreamUtils::parseAsObject(_stream, typeid(_var).name(), &_obj).good()) {
        return _stream;
    }

    QDspx::ParamCurve _tmpVar{};
    QAS::JsonStream _tmpStream;
    if (!(_tmpStream = QAS::JsonStreamUtils::parseObjectMember(_obj, "type", typeid(_tmpVar).name(),
                                                               &_tmpVar.type)).good()) {
        _stream.setStatus(_tmpStream.status());
        return _stream;
    }

    switch (_tmpVar.type) {
        case QDspx::ParamCurve::Anchor: {
            auto _realVar = QDspx::ParamAnchorRef::create();
            _var = _realVar;
            return _stream >> *_realVar.data();
        }
        case QDspx::ParamCurve::Free: {
            auto _realVar = QDspx::ParamFreeRef::create();
            _var = _realVar;
            return _stream >> *_realVar.data();
        }
    }

    return _stream;
}

QAS::JsonStream &QDspx::operator<<(QAS::JsonStream &_stream, const QDspx::ParamCurveRef &_var) {
    if (_var.isNull()) {
        return _stream;
    }

    return _var->type == QDspx::ParamCurve::Anchor ? (_stream << *_var.dynamicCast<QDspx::ParamAnchor>()) :
           (_stream << *_var.dynamicCast<QDspx::ParamFree>());
}