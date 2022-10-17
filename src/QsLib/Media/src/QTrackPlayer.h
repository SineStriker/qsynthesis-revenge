#ifndef QTRACKPLAYER_H
#define QTRACKPLAYER_H

#include <QObject>
#include <QtGlobal>

#include "qsmedia_global.h"

class QTrackPlayerPrivate;

class QSMEDIA_API QTrackPlayer : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QTrackPlayer)
public:
    QTrackPlayer(QObject *parent = nullptr);
    ~QTrackPlayer();

    int SampleRate;
    FF::SampleFormat Format;
    int Channels;

    int add(const QString &filename);
    void remove(int id);
    void clear();

protected:
    QTrackPlayer(QTrackPlayerPrivate &d, QObject *parent = nullptr);

    QScopedPointer<QTrackPlayerPrivate> d_ptr;
};

#endif // QTRACKPLAYER_H
