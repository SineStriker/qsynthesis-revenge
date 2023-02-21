#ifndef QDSPXTRACK_H
#define QDSPXTRACK_H

#include "QDspxNote.h"
#include "QDspxParam.h"

namespace QDspx {

    // 音轨区间时间信息
    struct DSCORE_API ClipTime {
        int start;
        int length;
        int clipStart;
        int clipLen;

        // 构造器
        ClipTime() : ClipTime(0, 4800){};

        ClipTime(int start, int length) : ClipTime(start, length, 0, length){};

        ClipTime(int start, int length, int clipStart, int clipLen)
            : start(start), length(length), clipStart(clipStart), clipLen(clipLen){};
    };

    // 音轨区间
    struct DSCORE_API Clip {
        enum Type {
            __qas_attr__("singing") //
            Singing,

            __qas_attr__("audio") //
            Audio,
        };

        QAS_JSON(Type)

        Type type;
        ClipTime time;
        QString name;
        Control control;

        // 不定长信息
        Extra extra;
        Workspace workspace;

        // 构造器
        Clip() : type(Singing){};
        explicit Clip(Type type) : type(type){};
        virtual ~Clip() = default;
    };

    using ClipRef = QSharedPointer<Clip>;

    // 波形区间
    struct DSCORE_API AudioClip : public Clip {
        QString path;

        // 构造器
        AudioClip() : Clip(Audio){};
    };

    using AudioClipRef = QSharedPointer<AudioClip>;

    // 人声区间
    struct DSCORE_API SingingClip : public Clip {
        QList<Note> notes;
        SingleParam params;

        // 不定长信息
        SourceInfo sources;

        // 构造器
        SingingClip() : Clip(Singing){};
    };

    using SingingClipRef = QSharedPointer<SingingClip>;

    // 音轨
    struct DSCORE_API Track {
        QString name;
        TrackControl control;
        QList<ClipRef> clips;

        // 不定长信息
        Extra extra;
        Workspace workspace;
    };

    QAS_JSON_NS(ClipTime)
    QAS_JSON_NS(Clip)
    QAS_JSON_NS(AudioClip)
    QAS_JSON_NS(SingingClip)
    QAS_JSON_NS(Track)
    QAS_JSON_NS_IMPL(ClipRef)

}

#endif // QDSPXTRACK_H
