#ifndef ADDITIVETREEMODELPRIVATE_H
#define ADDITIVETREEMODELPRIVATE_H

#include <QSet>

#include "AdditiveTreeModel.h"
#include "AdditiveTreeTransaction.h"

namespace QsApi {

    class AdditiveTreeItemPrivate {
        Q_DECLARE_PUBLIC(AdditiveTreeItem)
    public:
        AdditiveTreeItemPrivate();
        virtual ~AdditiveTreeItemPrivate();

        void init();

        void clearOld();

        AdditiveTreeItem *q_ptr;
        QString name;

        AdditiveTreeItem *parent;
        AdditiveTreeModel *model;
        int index;

        AdditiveTreeModel *oldModel;
        int oldIndex;

        QHash<QString, QVariant> properties;
        QByteArray bytes;
        QVector<AdditiveTreeItem *> vector;
        QSet<AdditiveTreeItem *> set;
    };

    class AdditiveTreeModelPrivate {
        Q_DECLARE_PUBLIC(AdditiveTreeModel)
    public:
        AdditiveTreeModelPrivate();
        virtual ~AdditiveTreeModelPrivate();

        void init();

        AdditiveTreeModel *q_ptr;
        bool is_destruct;

        QIODevice *dev;

        QHash<int, AdditiveTreeItem *> indexes;
        int maxIndex;

        AdditiveTreeItem *rootItem;

        AdditiveTreeTransaction *transaction;

        int addIndex(AdditiveTreeItem *item, int idx = -1);
        void removeIndex(int index);
    };

}

#endif // ADDITIVETREEMODELPRIVATE_H