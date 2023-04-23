//
// Created by Crs_1 on 2023/4/23.
//

#ifndef CHORUSKIT_MUSICTIMEPRIVATE_H
#define CHORUSKIT_MUSICTIMEPRIVATE_H

#include "MusicTime.h"

#include <QMap>

namespace ScriptMgr::Internal {

    class MusicTimePrivate : public QSharedData {
    public:
        QMap<int, double> timeSig;
        int tpqn;
        int measure;
        int beat;
        int tick;

        MusicTimePrivate() : tpqn(480), measure(0), beat(-1), tick(-1) {
            timeSig.insert(-std::numeric_limits<int>::lowest(), 4);
        }
    };

}

#endif // CHORUSKIT_MUSICTIMEPRIVATE_H
