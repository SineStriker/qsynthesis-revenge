#ifndef UTRACK_H
#define UTRACK_H

#include <QStringList>

/**
 *  Interpret from OpenUtau.Core.Ustx
 */

class UTrack {
public:
    UTrack();
    ~UTrack();

    void clear();

    QString singer;
    QString phonemizer;
    QString renderer;
    bool mute;
    bool solo;
    double volume;
};

#endif // UTRACK_H
