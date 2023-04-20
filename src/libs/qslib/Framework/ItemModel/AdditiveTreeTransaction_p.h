#ifndef ADDITIVETREETRANSACTIONPRIVATE_H
#define ADDITIVETREETRANSACTIONPRIVATE_H

#include <QPointer>
#include <QSharedData>
#include <QSharedDataPointer>

#include "AdditiveTreeTransaction.h"

namespace QsApi {

    class ATTData;

    class AdditiveTreeTransactionPrivate {
        Q_DECLARE_PUBLIC(AdditiveTreeTransaction);

    public:
        AdditiveTreeTransactionPrivate(AdditiveTreeTransaction *q, AdditiveTreeModel *model);
        ~AdditiveTreeTransactionPrivate();

        AdditiveTreeTransaction *q_ptr;
        AdditiveTreeModel *model;

        QVariantHash properties;
        QSharedDataPointer<ATTData> data;

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

    class ATTData : public QSharedData {
    public:
        ATTData();
        ATTData(const ATTData &other);
        ~ATTData();

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
        };

        struct RootChangeOp : public BaseOp {
            AdditiveTreeItem *oldRoot;
            AdditiveTreeItem *newRoot;
            RootChangeOp() : BaseOp(RootChange) {
            }
        };

        QPointer<AdditiveTreeModel> model;
        QList<BaseOp *> operations;

        static BaseOp *cloneOperation(BaseOp *op);
    };

    class ATTUndoCommand : public QUndoCommand {
    public:
        ATTUndoCommand();
        ~ATTUndoCommand();

        void undo() override;
        void redo() override;

        int done;
        QSharedDataPointer<ATTData> data;

    private:
        bool execute(bool undo) const;
    };

}

#endif // ADDITIVETREETRANSACTIONPRIVATE_H