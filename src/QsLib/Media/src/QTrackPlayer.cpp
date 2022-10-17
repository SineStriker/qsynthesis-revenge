#include "QTrackPlayer.h"
#include "QTrackPlayer_p.h"

#include <QFile>

QTrackPlayer::QTrackPlayer(QObject *parent) : QTrackPlayer(*new QTrackPlayerPrivate(), parent) {
}

QTrackPlayer::~QTrackPlayer() {
}

int QTrackPlayer::add(const QString &filename) {
    // Wave file

    Q_D(QTrackPlayer);

    QTrackPlayerPrivate::AudioMeta meta(new QFile(filename));
    auto &file = *meta.file;
    if (!file.open(QIODevice::ReadOnly)) {
        return -1;
    }

    d->maxId++;
    d->openedAudios.insert(d->maxId, meta);

    return d->maxId;
}

void QTrackPlayer::remove(int id) {
    Q_D(QTrackPlayer);
    auto it = d->openedAudios.find(id);
    if (it == d->openedAudios.end()) {
        return;
    }
    // close by destructor
    d->openedAudios.erase(it);
}

void QTrackPlayer::clear() {
    Q_D(QTrackPlayer);
    // close by destructor
    d->openedAudios.clear();
}

QTrackPlayer::QTrackPlayer(QTrackPlayerPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();

    SampleRate = 44100;
    Format = FF::FMT_S16;
    Channels = 2;
}
