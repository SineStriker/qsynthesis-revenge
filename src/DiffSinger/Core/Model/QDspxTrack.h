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
        ClipTime() : ClipTime(0, 4800) {};

        ClipTime(int start, int length) : ClipTime(start, length, 0, length) {};

        ClipTime(int start, int length, int clipStart, int clipLen)
                : start(start), length(length), clipStart(clipStart), clipLen(clipLen) {};
    };

    // 音轨区间
    struct DSCORE_API Clip {
        enum Type {
            QAS_ATTRIBUTE("singing")
            Singing,

            QAS_ATTRIBUTE("audio")
            Audio,
        };

        Type type;
        ClipTime time;
        QString name;
        Control control;

        // 不定长信息
        Extra extra;
        Workspace workspace;

        // 构造器
        Clip() : type(Singing) {};

        explicit Clip(Type type) : type(type) {};

        virtual ~Clip() = default;
    };

    using ClipRef = QSharedPointer<Clip>;

    // 波形区间
    struct DSCORE_API AudioClip : public Clip {
        QString path;

        // 构造器
        AudioClip() : Clip(Audio) {};
    };

    using AudioClipRef = QSharedPointer<AudioClip>;

    // 人声区间
    struct DSCORE_API SingingClip : public Clip {
        QList<Note> notes;
        SingleParam params;

        // 不定长信息
        SourceInfo sources;

        // 构造器
        SingingClip() : Clip(Singing) {};
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

} // namespace QDspx

QAS_JSON_DECLARE(QDspx::ClipTime)

QAS_JSON_DECLARE(QDspx::Clip)

QAS_JSON_DECLARE(QDspx::AudioClip)

QAS_JSON_DECLARE(QDspx::SingingClip)

QAS_JSON_DECLARE(QDspx::Track)

QAS_ENUM_DECLARE(QDspx::Clip::Type)

QAS_JSON_DECLARE_IMPL(QDspx::ClipRef)

#endif // QDSPXTRACK_H
