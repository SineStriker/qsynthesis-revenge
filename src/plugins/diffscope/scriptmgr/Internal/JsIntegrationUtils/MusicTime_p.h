//
// Created by Crs_1 on 2023/4/23.
//

#ifndef CHORUSKIT_MUSICTIMEPRIVATE_H
#define CHORUSKIT_MUSICTIMEPRIVATE_H

#include "MusicTime.h"

#include <QMap>

namespace ScriptMgr::Internal {

    class MusicTimePrivate {
        Q_DECLARE_PUBLIC(MusicTime)
    public:
        MusicTime *q_ptr;
        QMap<int, double> timeSig;
        int tpqn;
        int measure;
        int beat;
        int tick;
    };

}

#endif // CHORUSKIT_MUSICTIMEPRIVATE_H
