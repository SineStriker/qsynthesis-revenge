#ifndef PIANOSPEC_H
#define PIANOSPEC_H

#include <QString>
#include <QColor>

namespace PianoSpec {

    struct LanguageSpec {
        QString shortname;
        QColor color;
    };

    struct SpeakerDesc {
        QString id;
        QString name;
    };

    struct ItemDesc {
        QString id;
        QString name;
        QString speaker;
        bool isDir;
        LanguageSpec lang;
    };

}

#endif // PIANOSPEC_H
