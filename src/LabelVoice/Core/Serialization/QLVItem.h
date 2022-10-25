#ifndef QLVITEM_H
#define QLVITEM_H

#include <QMap>
#include <QStringList>

namespace LVModel {

    class SliceDefinition {
    public:
        QString Id;
        double In;
        double Out;
        QString Language;

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
    };

} // namespace LVModel

#endif // QLVITEM_H
