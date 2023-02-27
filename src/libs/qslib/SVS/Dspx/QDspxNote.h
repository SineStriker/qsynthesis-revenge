#ifndef QDSPXNOTE_H
#define QDSPXNOTE_H

#include "QDspxBase.h"

namespace QDspx {

    // 音素
    struct QDSPX_EXPORT Phoneme {
        enum Type {
            __qas_attr__("ahead")  //
            Ahead,
            __qas_attr__("normal") //
            Normal,
            __qas_attr__("final")  //
            Final,
        };
        QAS_JSON(Type)

        Type type;
        QString token;
        int duration;

        // 不定长信息
        Extra extra;
        Workspace workspace;

        // 构造器
        Phoneme() : type(Normal), duration(0){};
    };

    // 音素信息
    struct QDSPX_EXPORT PhonemeInfo {
        __qas_attr__("original")
        QList<Phoneme> org;
        
        QList<Phoneme> edited;
    };

    // 颤音信息
    struct QDSPX_EXPORT VibratoInfo {
        double start;
        double end;
        double freq;
        double phase;
        double amp;
        double offset;
        QList<DoublePoint> points;

        // 构造器
        VibratoInfo() : start(0), end(1), freq(2), phase(0), amp(1), offset(0){};
    };

    // 音符
    struct QDSPX_EXPORT Note {
        int pos;
        int length;
        int keyNum;
        QString lyric;
        PhonemeInfo phonemes;
        VibratoInfo vibrato;

        // 不定长信息
        Extra extra;
        Workspace workspace;

        // 构造器
        Note() : Note(0, 480, 60){};
        Note(int pos, int length, int keyNum) : pos(pos), length(length), keyNum(keyNum){};
    };

    QAS_JSON_NS(Phoneme)
    QAS_JSON_NS(PhonemeInfo)
    QAS_JSON_NS(VibratoInfo)
    QAS_JSON_NS(Note)

}

#endif // QDSPXNOTE_H
