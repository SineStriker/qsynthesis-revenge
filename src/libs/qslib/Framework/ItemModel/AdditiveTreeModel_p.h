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
        QString name;

        AdditiveTreeItem *parent;
        AdditiveTreeModel *model;
        int index;

        QHash<QString, QVariant> properties;
        QByteArray bytes;
        QVector<AdditiveTreeItem *> vector;
        QSet<AdditiveTreeItem *> set;

        void setProperty_helper(const QString &key, const QVariant &value);
        void setBytes_helper(int start, const QByteArray &bytes);
        void truncateBytes_helper(int size);
        void insertRows_helper(int index, const QVector<AdditiveTreeItem *> &items);
        void moveRows_helper(int index, int count, int dest);
        void removeRows_helper(int index, int count);
        void addUnique_helper(AdditiveTreeItem *item);
        void removeUnique_helper(AdditiveTreeItem *item);
    };

    class AdditiveTreeModelPrivate {
        Q_DECLARE_PUBLIC(AdditiveTreeModel)
    public:
        AdditiveTreeModelPrivate();
        virtual ~AdditiveTreeModelPrivate();

        void init();

        AdditiveTreeModel *q_ptr;
        bool is_destruct;

        QHash<int, AdditiveTreeItem *> indexes;
        int maxIndex;

        AdditiveTreeItem *rootItem;

        void setRootItem_helper(AdditiveTreeItem *item);

        int addIndex(AdditiveTreeItem *item, int idx = -1);
        void removeIndex(int index);

        // Operations
        enum Change {
            PropertyChange,
            BytesSet,
            BytesTruncate,
            RowsInsert,
            RowsMove,
            RowsRemove,
            UniqueAdd,
            UniqueRemove,
            RootChange,
        };

        struct BaseOp {
            Change c;
            explicit BaseOp(Change c) : c(c){};
            virtual ~BaseOp() = default;
        };

        struct PropertyChangeOp : public BaseOp {
            int id;
            QString key;
            QVariant oldValue;
            QVariant newValue;
            PropertyChangeOp() : BaseOp(PropertyChange) {
            }
        };

        struct BytesSetOp : public BaseOp {
            int id;
            int start;
            QByteArray oldBytes;
            QByteArray newBytes;
            BytesSetOp() : BaseOp(BytesSet) {
            }
        };

        struct BytesTruncateOp : public BaseOp {
            int id;
            int size;
            QByteArray oldBytes;
            int delta;
            BytesTruncateOp() : BaseOp(BytesTruncate) {
            }
        };

        struct RowsInsertRemoveOp : public BaseOp {
            int id;
            int index;
            QVector<AdditiveTreeItem *> items;
            RowsInsertRemoveOp(bool isInsert = false) : BaseOp(isInsert ? RowsInsert : RowsRemove) {
            }
            ~RowsInsertRemoveOp() {
                for (const auto &item : qAsConst(items))
                    if (!item->model())
                        delete item;
            }
        };

        struct RowsMoveOp : public BaseOp {
            int id;
            int index;
            int count;
            int dest;
            RowsMoveOp() : BaseOp(RowsMove) {
            }
        };

        struct UniqueAddRemoveOp : public BaseOp {
            int id;
            AdditiveTreeItem *item;
            UniqueAddRemoveOp(bool isInsert = false) : BaseOp(isInsert ? UniqueAdd : UniqueRemove) {
            }
            ~UniqueAddRemoveOp() {
                if (!item->model())
                    delete item;
            }
        };

        struct RootChangeOp : public BaseOp {
            AdditiveTreeItem *oldRoot;
            AdditiveTreeItem *newRoot;
            RootChangeOp() : BaseOp(RootChange) {
            }
            ~RootChangeOp() {
                if (oldRoot && !oldRoot->model())
                    delete oldRoot;
                if (newRoot && !newRoot->model())
                    delete newRoot;
            }
        };

        int stepIndex;
        QVector<BaseOp *> operations;
        bool internalChange;

        bool execute(BaseOp *op, bool undo);
        void push(BaseOp *op);

    public:
        void propertyChanged(AdditiveTreeItem *item, const QString &key, const QVariant &oldValue,
                             const QVariant &newValue);

        void bytesSet(AdditiveTreeItem *item, int start, const QByteArray &oldBytes, const QByteArray &newBytes);
        void bytesTruncated(AdditiveTreeItem *item, int size, const QByteArray &oldBytes, int delta);

        void rowsInserted(AdditiveTreeItem *parent, int index, const QVector<AdditiveTreeItem *> &items);
        void rowsMoved(AdditiveTreeItem *parent, int index, int count, int dest);
        void rowsRemoved(AdditiveTreeItem *parent, int index, const QVector<AdditiveTreeItem *> &items);

        void uniqueAdded(AdditiveTreeItem *parent, AdditiveTreeItem *item);
        void uniqueRemoved(AdditiveTreeItem *parent, AdditiveTreeItem *item);

        void rootChanged(AdditiveTreeItem *oldRoot, AdditiveTreeItem *newRoot);
    };

}

#endif // ADDITIVETREEMODELPRIVATE_H