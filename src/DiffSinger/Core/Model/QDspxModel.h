#ifndef QDSPXMODEL_H
#define QDSPXMODEL_H

#include "QDspxTimeline.h"
#include "QDspxTrack.h"

namespace QDspx {

    // 文件的元信息，包括版本号、工程名、作者等
    struct Metadata {
        QString version;
        QString name;
        QString author;
    };

    // 循环区间
    struct LoopRange {
        bool enabled;
        int start;
        int length;

        // 构造器
        LoopRange() : LoopRange(false, 0, 0){};
        LoopRange(bool enabled, int start, int length)
            : enabled(enabled), start(start), length(length){};
    };

    // 总线控制
    struct Master {
        Control control;
        LoopRange loop;
    };

    // 工程可编辑区域
    struct Content {
        Master master;
        Timeline timeline;
        QList<Track> tracks;

        // 不定长信息
        Extra extra;
        Workspace workspace;
    };

    // 工程
    struct Model {
        Metadata metadata;
        Content content;

        // 不定长信息
        Workspace workspace;
    };

    DSCORE_API bool fromMidi(const QString &filename, Model *out);

} // namespace QDspx

using QDspxModel = QDspx::Model;

#endif // QDSPXMODEL_H
