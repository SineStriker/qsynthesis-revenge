#ifndef QDSPXTIMELINE_H
#define QDSPXTIMELINE_H

#include "QDspxBase.h"

namespace QDspx {

    // 拍号
    struct QDSPX_EXPORT TimeSignature {
        int pos;

        __qas_attr__("numerator")
        int num;

        __qas_attr__("denominator")
        int den;

        // 构造器
        TimeSignature() : TimeSignature(0, 4, 4){};
        TimeSignature(int pos, int num, int den) : pos(pos), num(num), den(den){};
    };

    // 曲速
    struct QDSPX_EXPORT Tempo {
        int pos;
        double value;

        // 构造器
        Tempo() : Tempo(0, 120){};
        Tempo(int pos, double value) : pos(pos), value(value){};
    };

    // 曲速
    struct QDSPX_EXPORT Label {
        int pos;
        QString text;

        // 构造器
        Label() : Label(0){};
        Label(int pos, const QString &text = QString()) : pos(pos), text(text){};
    };

    // 时间线
    struct QDSPX_EXPORT Timeline {
        QList<TimeSignature> timeSignatures;
        QList<Tempo> tempos;
        QList<Label> labels;
    };

    QAS_JSON_NS(TimeSignature)
    QAS_JSON_NS(Tempo)
    QAS_JSON_NS(Label)
    QAS_JSON_NS(Timeline)

}

#endif // QDSPXTIMELINE_H
