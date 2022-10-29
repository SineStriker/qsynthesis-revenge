#ifndef PIANOSPEC_H
#define PIANOSPEC_H

#include <QString>

namespace PianoSpec {

    struct SpeakerDesc {
        QString id;
        QString name;
    };

    struct ItemDesc {
        QString id;
        QString name;
        QString speaker;
        bool isDir;
    };

} // namespace PianoSpec

#endif // PIANOSPEC_H
