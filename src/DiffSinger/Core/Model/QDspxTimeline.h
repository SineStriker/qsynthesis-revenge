#ifndef QDSPXTIMELINE_H
#define QDSPXTIMELINE_H

#include "QDspxBase.h"

namespace QDspx {

    // 拍号
    struct DSCORE_API TimeSignature {
        int pos;
        int num;
        int den;

        // 构造器
        TimeSignature() : TimeSignature(0, 4, 4){};
        TimeSignature(int pos, int num, int den) : pos(pos), num(num), den(den){};
    };

    // 曲速
    struct DSCORE_API Tempo {
        int pos;
        double value;

        // 构造器
        Tempo() : Tempo(0, 120){};
        Tempo(int pos, double value) : pos(pos), value(value){};
    };

    // 曲速
    struct DSCORE_API Label {
        int pos;
        QString text;

        // 构造器
        Label() : Label(0){};
        Label(int pos, const QString &text = QString()) : pos(pos), text(text){};
    };

    // 时间线
    struct DSCORE_API Timeline {
        QList<TimeSignature> timeSignatures;
        QList<Tempo> tempos;
        QList<Label> labels;
    };

} // namespace QDspx

#endif // QDSPXTIMELINE_H
