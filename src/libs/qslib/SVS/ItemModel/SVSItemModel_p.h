#ifndef SVSITEMMODELPRIVATE_H
#define SVSITEMMODELPRIVATE_H

#include "SVSItemModel.h"

namespace QsApi {

    class SVSItemPrivate {
        Q_DECLARE_PUBLIC(SVSItem)
    public:
        SVSItemPrivate();
        virtual ~SVSItemPrivate();

        void init();

        SVSItem *q_ptr;

        int type;

        SVSItem *parent;
        SVSItemModel *model;

        Qt::ItemFlags flags;
    };

    class SVSSequentialItemPrivate : public SVSItemPrivate {
        Q_DECLARE_PUBLIC(SVSSequentialItem)
    public:
        SVSSequentialItemPrivate();
        ~SVSSequentialItemPrivate();

        void init();

        QVector<SVSItem *> children;
    };

    class SVSTimeOrderedItemPrivate : public SVSItemPrivate {
        Q_DECLARE_PUBLIC(SVSTimeOrderedItem)
    public:
        SVSTimeOrderedItemPrivate();
        ~SVSTimeOrderedItemPrivate();

        void init();
    };

    class SVSMapItemPrivate : public SVSItemPrivate {
        Q_DECLARE_PUBLIC(SVSMapItem)
    public:
        SVSMapItemPrivate();
        ~SVSMapItemPrivate();

        void init();
    };

    class SVSSegmentedItemPrivate : public SVSItemPrivate {
        Q_DECLARE_PUBLIC(SVSSegmentedItem)
    public:
        SVSSegmentedItemPrivate();
        ~SVSSegmentedItemPrivate();

        void init();
    };

    class SVSItemModelPrivate {
        Q_DECLARE_PUBLIC(SVSItemModel)
    public:
        SVSItemModelPrivate();
        virtual ~SVSItemModelPrivate();

        void init();

        SVSItemModel *q_ptr;
    };

}

#endif // SVSITEMMODELPRIVATE_H