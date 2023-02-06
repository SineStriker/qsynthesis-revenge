#ifndef QDSPXNOTE_H
#define QDSPXNOTE_H

#include "QDspxBase.h"

namespace QDspx {

    // 音素
    class Phoneme {
        enum Type {
            Ahead,
            Normal,
            Final,
        };

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
    struct PhonemeInfo {
        QList<Phoneme> org;
        QList<Phoneme> edited;
    };

    // 颤音信息
    struct VibratoInfo {
        double start;
        double end;
        double freq;
        double phase;
        double amp;
        double offset;
        QList<IntPoint> points;

        // 构造器
        VibratoInfo() : start(0), end(1), freq(2), phase(0), amp(1), offset(0){};
    };

    // 音符
    struct Note {
        int pos;
        int length;
        int keyNum;
        QString lyric;
        PhonemeInfo phonemes;
        VibratoInfo vibrato;

        // 不定长信息
        Extra Extra;
        Workspace Workspace;

        // 构造器
        Note() : Note(0, 480, 60){};
        Note(int pos, int length, int keyNum) : pos(pos), length(length), keyNum(keyNum){};
    };

} // namespace QDspx

#endif // QDSPXNOTE_H
