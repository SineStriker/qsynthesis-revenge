#ifndef QDSPXNOTE_H
#define QDSPXNOTE_H

#include "QDspxBase.h"

namespace QDspx {

    // 音素
    struct DSCORE_API Phoneme {
        enum Type {
            QAS_ATTRIBUTE("ahead")
            Ahead,

            QAS_ATTRIBUTE("normal")
            Normal,

            QAS_ATTRIBUTE("final")
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
    struct DSCORE_API PhonemeInfo {
        QAS_ATTRIBUTE("original")
        QList<Phoneme> org;

        QList<Phoneme> edited;
    };

    // 颤音信息
    struct DSCORE_API VibratoInfo {
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
    struct DSCORE_API Note {
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

} // namespace QDspx

QAS_JSON_DECLARE(QDspx::Phoneme)

QAS_JSON_DECLARE(QDspx::PhonemeInfo)

QAS_JSON_DECLARE(QDspx::VibratoInfo)

QAS_JSON_DECLARE(QDspx::Note)

QAS_ENUM_DECLARE(QDspx::Phoneme::Type)

#endif // QDSPXNOTE_H
