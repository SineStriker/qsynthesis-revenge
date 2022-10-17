#ifndef QTRACKPLAYERPRIVATE_H
#define QTRACKPLAYERPRIVATE_H

#include "QTrackPlayer.h"

#include <QFile>
#include <QMap>
#include <QSharedPointer>

class QTrackPlayerPrivate {
    Q_DECLARE_PUBLIC(QTrackPlayer)
public:
    QTrackPlayerPrivate();
    virtual ~QTrackPlayerPrivate();

    struct AudioMeta {
        QSharedPointer<QFile> file;
        int entry;

        AudioMeta() = default;
        AudioMeta(QFile *f) : file(f) {
        }
    };

    void init();

    QTrackPlayer *q_ptr;

    int maxId;
    QMap<int, AudioMeta> openedAudios;
};

#endif // QTRACKPLAYERPRIVATE_H
