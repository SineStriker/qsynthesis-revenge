#ifndef QDSPXPARAM_H
#define QDSPXPARAM_H

#include "QDspxBase.h"

namespace QDspx {
    // 参数曲线基类
    struct QDSPX_EXPORT ParamCurve {
        enum Type {
            __qas_attr__("anchor") //
            Anchor,
            __qas_attr__("free")   //
            Free,
        };
        QAS_JSON(Type)

        Type type;

        // 构造器
        ParamCurve() : ParamCurve(Anchor){};
        explicit ParamCurve(Type type) : type(type){};
        virtual ~ParamCurve() = default;
    };

    using ParamCurveRef = QSharedPointer<ParamCurve>;

    // 手绘参数
    struct QDSPX_EXPORT ParamFree : public ParamCurve {
        int start;
        int step;
        QList<int> values;

        // 构造器
        ParamFree() : ParamFree(0){};
        explicit ParamFree(int start) : ParamCurve(Free), start(start), step(5){};
    };

    using ParamFreeRef = QSharedPointer<ParamFree>;

    // 锚点参数
    struct QDSPX_EXPORT ParamAnchor : public ParamCurve {
        QList<AnchorPoint> nodes;

        // 构造器
        ParamAnchor() : ParamCurve(Anchor){};
    };

    using ParamAnchorRef = QSharedPointer<ParamAnchor>;

    // 参数结构
    struct QDSPX_EXPORT ParamInfo {
        __qas_attr__("original")
        QList<ParamCurveRef> org;

        QList<ParamCurveRef> edited;
        
        QList<ParamCurveRef> envelope;
    };

    // 单线条参数
    struct QDSPX_EXPORT SingleParam {
        ParamInfo pitch;
        ParamInfo energy;
    };

    QAS_JSON_NS(ParamCurve)
    QAS_JSON_NS(ParamFree)
    QAS_JSON_NS(ParamAnchor)
    QAS_JSON_NS(ParamInfo)
    QAS_JSON_NS(SingleParam)
    QAS_JSON_NS_IMPL(ParamCurveRef)

}

#endif // QDSPXPARAM_H
