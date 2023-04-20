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
        bool is_destruct;

        bool inTransaction;
        QIODevice *dev;

        QHash<int, AdditiveTreeItem *> indexes;
        int maxIndex;

        AdditiveTreeItem *rootItem;

        int addIndex(AdditiveTreeItem *item);
        void removeIndex(int index);

        void propertyChanged(AdditiveTreeItem *item, const QString &key, const QVariant &oldValue,
                             const QVariant &newValue);

        void bytesSet(AdditiveTreeItem *item, int start, int len, const QByteArray &oldBytes,
                      const QByteArray &newBytes);
        void bytesTruncated(AdditiveTreeItem *item, int size, const QByteArray &oldBytes);

        void rowInserted(AdditiveTreeItem *parent, int index, const QList<AdditiveTreeItem *> &items);
        void rowMoved(AdditiveTreeItem *parent, int index, int count, int dest);
        void rowRemoved(AdditiveTreeItem *parent, int index, const QList<AdditiveTreeItem *> &items);

        void uniqueAdded(AdditiveTreeItem *parent, AdditiveTreeItem *item);
        void uniqueRemoved(AdditiveTreeItem *parent, AdditiveTreeItem *item);
    };

}

#endif // ADDITIVETREEMODELPRIVATE_H