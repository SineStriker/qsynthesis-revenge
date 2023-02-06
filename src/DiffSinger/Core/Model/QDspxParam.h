#ifndef QDSPXPARAM_H
#define QDSPXPARAM_H

#include "QDspxBase.h"

namespace QDspx {

    // 参数曲线基类
    struct ParamCurve {
        enum Type {
            Anchor,
            Free,
        };

        Type type;

        // 构造器
        explicit ParamCurve(Type type) : type(type){};

        // 转换函数
        DSCORE_API static QString TypeToString(Type type);
        DSCORE_API static Type StringToType(const QString &s);
    };

    using ParamCurveRef = QSharedPointer<ParamCurve>;

    // 手绘参数
    struct ParamFree : public ParamCurve {
        int start;
        int step;
        QList<int> values;

        // 构造器
        explicit ParamFree(int start = 0) : ParamCurve(Free), start(start), step(5){};
    };

    using ParamFreeRef = QSharedPointer<ParamFree>;

    // 锚点参数
    struct ParamAnchor : public ParamCurve {
        QList<AnchorPoint> nodes;

        // 构造器
        ParamAnchor() : ParamCurve(Anchor){};
    };

    using ParamAnchorRef = QSharedPointer<ParamAnchor>;

    // 参数结构
    struct ParamInfo {
        QList<ParamCurveRef> org;
        QList<ParamCurveRef> edited;
        QList<ParamCurveRef> envelope;
    };

    // 单线条参数
    struct SingleParam {
        ParamInfo pitch;
        ParamInfo energy;
    };

} // namespace QDspx

#endif // QDSPXPARAM_H
