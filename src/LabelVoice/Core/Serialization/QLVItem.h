#ifndef QLVITEM_H
#define QLVITEM_H

#include <QMap>
#include <QStringList>

#include "Utils/HexGenerator.h"

namespace LVModel {

    class SliceDefinition {
    public:
        QString Id;
        double In;
        double Out;
        QString Language;

        SliceDefinition();
        ~SliceDefinition();

        bool operator<(const SliceDefinition &other) const;
    };

    class ItemModel {
    public:
        QString Version;
        QString Id;
        QString Name;
        QString OwnerProject;
        QString AudioSource;
        QMap<QString, SliceDefinition> Slices;

        ItemModel(HexGenerator &hexGen);
        ~ItemModel();

    public:
        bool load(const QString &filename);
        bool save(const QString &filename);

        void reset();

        void Validate(bool reg = false);

    protected:
        HexGenerator &registry;
    };

} // namespace LVModel

#endif // QLVITEM_H
