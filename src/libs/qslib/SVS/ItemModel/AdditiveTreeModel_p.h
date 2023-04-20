#ifndef ADDITIVETREEMODELPRIVATE_H
#define ADDITIVETREEMODELPRIVATE_H

#include <QSet>

#include "AdditiveTreeModel.h"

namespace QsApi {

    class AdditiveTreeItemPrivate {
        Q_DECLARE_PUBLIC(AdditiveTreeItem)
    public:
        AdditiveTreeItemPrivate();
        virtual ~AdditiveTreeItemPrivate();

        void init();

        AdditiveTreeItem *q_ptr;

        AdditiveTreeItem *parent;
        AdditiveTreeModel *model;
        int index;

        QString name;
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

        bool inTransaction;
        QIODevice *dev;

        QHash<int, AdditiveTreeModel *> indexes;
        int maxIndex;

        AdditiveTreeItem *rootItem;
    };

}

#endif // ADDITIVETREEMODELPRIVATE_H