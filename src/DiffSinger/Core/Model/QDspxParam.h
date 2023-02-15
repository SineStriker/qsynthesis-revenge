#ifndef QDSPXPARAM_H
#define QDSPXPARAM_H

#include "QDspxBase.h"

namespace QDspx {

    // 参数曲线基类
    struct DSCORE_API ParamCurve {
        enum Type {
            QAS_ATTRIBUTE("anchor")
            Anchor,

            QAS_ATTRIBUTE("free")
            Free,
        };

        Type type;

        // 构造器
        ParamCurve() : ParamCurve(Anchor){};
        explicit ParamCurve(Type type) : type(type){};
        virtual ~ParamCurve() = default;
    };

    using ParamCurveRef = QSharedPointer<ParamCurve>;

    // 手绘参数
    struct DSCORE_API ParamFree : public ParamCurve {
        int start;
        int step;
        QList<int> values;

        // 构造器
        ParamFree() : ParamFree(0){};
        explicit ParamFree(int start) : ParamCurve(Free), start(start), step(5){};
    };

    using ParamFreeRef = QSharedPointer<ParamFree>;

    // 锚点参数
    struct DSCORE_API ParamAnchor : public ParamCurve {
        QList<AnchorPoint> nodes;

        // 构造器
        ParamAnchor() : ParamCurve(Anchor){};
    };

    using ParamAnchorRef = QSharedPointer<ParamAnchor>;

    // 参数结构
    struct DSCORE_API ParamInfo {
        QAS_ATTRIBUTE("original")
        QList<ParamCurveRef> org;

        QList<ParamCurveRef> edited;

        QList<ParamCurveRef> envelope;
    };

    // 单线条参数
    struct DSCORE_API SingleParam {
        ParamInfo pitch;
        ParamInfo energy;
    };

} // namespace QDspx

QAS_JSON_DECLARE(QDspx::ParamCurve)

QAS_JSON_DECLARE(QDspx::ParamFree)

QAS_JSON_DECLARE(QDspx::ParamAnchor)

QAS_JSON_DECLARE(QDspx::ParamInfo)

QAS_JSON_DECLARE(QDspx::SingleParam)

QAS_ENUM_DECLARE(QDspx::ParamCurve::Type)

QAS_JSON_DECLARE_IMPL(QDspx::ParamCurveRef)

#endif // QDSPXPARAM_H
