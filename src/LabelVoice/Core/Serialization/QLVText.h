#ifndef QLVTEXT_H
#define QLVTEXT_H

#include <QMap>
#include <QSet>
#include <QStringList>

#include <set>

#include "Utils/HexGenerator.h"

namespace LVModel {

    class AudioSourceReference {
    public:
        QString Path;
        double In;
        double Out;

        AudioSourceReference();
        ~AudioSourceReference();
    };


    class Boundary {
    public:
        int Id;
        double Position;
        QString Text;

        Boundary();
        ~Boundary();

        bool operator<(const Boundary &other) const {
            return Position < other.Position;
        }
    };

    class TextLayer {
    public:
        QString Name;
        std::set<Boundary> Boundaries;
    };

    class TextLabelModel {
        TextLabelModel();
        ~TextLabelModel();

    public:
        bool load(const QString &filename);
        bool save(const QString &filename);

        void reset();

    public:
        int maxIdReached() const;

        void Validate();

        inline int NextId() {
            return ++_maxId;
        }

        int Version;
        AudioSourceReference AudioSource;
        QList<TextLayer> Layers;
        QList<std::set<int>> BindingGroups;

    protected:
        QSet<int> _ids;
        int _maxId;
    };

} // namespace LVModel

#endif // QLVTEXT_H
