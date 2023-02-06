#ifndef QDSPXTRACK_H
#define QDSPXTRACK_H

#include "QDspxNote.h"
#include "QDspxParam.h"

namespace QDspx {
    
    // 音轨区间时间信息
    struct ClipTime {
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
    struct Clip {
        enum Type {
            Singing,
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
        explicit Clip(Type type) : type(type){};

        // 转换函数
        DSCORE_API static QString TypeToString(Type type);
        DSCORE_API static Type StringToType(const QString &s);
    };

    using ClipRef = QSharedPointer<Clip>;

    // 波形区间
    struct AudioClip : public Clip {
        QString path;

        // 构造器
        AudioClip() : Clip(Audio){};
    };

    using AudioClipRef = QSharedPointer<AudioClip>;

    // 人声区间
    struct SingingClip : public Clip {
        QList<Note> notes;
        SingleParam params;

        // 不定长信息
        SourceInfo sources;

        // 构造器
        SingingClip() : Clip(Singing){};
    };

    using SingingClipRef = QSharedPointer<SingingClip>;

    // 音轨
    struct Track {
        QString name;
        TrackControl control;
        QList<ClipRef> clips;
        
        // 不定长信息
        Extra extra;
        Workspace workspace;
    };

} // namespace QDspx

#endif // QDSPXTRACK_H
