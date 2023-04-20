#include "AdditiveTreeTransaction.h"
#include "AdditiveTreeTransaction_p.h"

#include "AdditiveTreeModel_p.h"

namespace QsApi {

    template <typename ForwardIterator>
    Q_OUTOFLINE_TEMPLATE void qCloneAll(ForwardIterator begin, ForwardIterator end, ForwardIterator dest) {
        while (begin != end) {
            *dest = (*begin)->clone();
            ++begin;
            ++dest;
        }
    }

    template <template <class> class Container1, template <class> class Container2, class T>
    Container2<T> qCloneAll(const Container1<T> &container) {
        Container2<T> res;
        res.reserve(container.size());
        for (const auto &item : container)
            res.push_back(item->clone());
        return res;
    }

    void AdditiveTreeTransactionPrivate::propertyChanged(AdditiveTreeItem *item, const QString &key,
                                                         const QVariant &oldValue, const QVariant &newValue) {
        auto op = new ATTData::PropertyChangeOp();
        op->id = item->index();
        op->key = key;
        op->oldValue = oldValue;
        op->newValue = newValue;
        data->operations.append(op);
    }

    void AdditiveTreeTransactionPrivate::bytesSet(AdditiveTreeItem *item, int start, const QByteArray &oldBytes,
                                                  const QByteArray &newBytes) {
        auto op = new ATTData::BytesSetOp();
        op->id = item->index();
        op->start = start;
        op->oldBytes = oldBytes;
        op->newBytes = newBytes;
        data->operations.append(op);
    }

    void AdditiveTreeTransactionPrivate::bytesTruncated(AdditiveTreeItem *item, int size, const QByteArray &oldBytes,
                                                        int delta) {
        auto op = new ATTData::BytesTruncateOp();
        op->id = item->index();
        op->size = size;
        op->oldBytes = oldBytes;
        op->delta = delta;
        data->operations.append(op);
    }

    void AdditiveTreeTransactionPrivate::rowsInserted(AdditiveTreeItem *parent, int index,
                                                      const QVector<AdditiveTreeItem *> &items) {
        auto op = new ATTData::RowsInsertRemoveOp(true);
        op->id = parent->index();
        op->index = index;
        op->items = qCloneAll<QVector, QVector>(items);
        data->operations.append(op);
    }

    void AdditiveTreeTransactionPrivate::rowsMoved(AdditiveTreeItem *parent, int index, int count, int dest) {
        auto op = new ATTData::RowsMoveOp();
        op->id = parent->index();
        op->index = index;
        op->count = count;
        op->dest = dest;
        data->operations.append(op);
    }

    void AdditiveTreeTransactionPrivate::rowsRemoved(AdditiveTreeItem *parent, int index,
                                                     const QVector<AdditiveTreeItem *> &items) {
        auto op = new ATTData::RowsInsertRemoveOp(false);
        op->id = parent->index();
        op->index = index;
        op->items = items; // No use clone
        data->operations.append(op);
    }

    void AdditiveTreeTransactionPrivate::uniqueAdded(AdditiveTreeItem *parent, AdditiveTreeItem *item) {
        auto op = new ATTData::UniqueAddRemoveOp(true);
        op->id = parent->index();
        op->item = item->clone();
        data->operations.append(op);
    }

    void AdditiveTreeTransactionPrivate::uniqueRemoved(AdditiveTreeItem *parent, AdditiveTreeItem *item) {
        auto op = new ATTData::UniqueAddRemoveOp(false);
        op->id = parent->index();
        op->item = item; // No use clone
        data->operations.append(op);
    }

    void AdditiveTreeTransactionPrivate::rootChanged(AdditiveTreeItem *oldRoot, AdditiveTreeItem *newRoot) {
        auto op = new ATTData::RootChangeOp();
        op->oldRoot = oldRoot->clone();
        op->newRoot = newRoot->clone();
        data->operations.append(op);
    }

    AdditiveTreeTransaction::AdditiveTreeTransaction(AdditiveTreeModel *model)
        : d_ptr(new AdditiveTreeTransactionPrivate(this, model)) {
        model->d_func()->transaction = this;
    }

    AdditiveTreeTransaction::~AdditiveTreeTransaction() {
        d_ptr->model->d_func()->transaction = nullptr;
    }

    void AdditiveTreeTransaction::read(QDataStream &in) {
    }

    void AdditiveTreeTransaction::write(QDataStream &out) const {
    }

    QVariant AdditiveTreeTransaction::property(const QString &key) const {
        return d_ptr->properties.value(key, {});
    }

    void AdditiveTreeTransaction::setProperty(const QString &key, const QVariant &value) {
        d_ptr->properties[key] = value;
    }

    QUndoCommand *AdditiveTreeTransaction::createCommand() const {
        auto cmd = new ATTUndoCommand();
        cmd->data = d_ptr->data;
        return cmd;
    }

    // Private
    AdditiveTreeTransactionPrivate::AdditiveTreeTransactionPrivate(AdditiveTreeTransaction *q, AdditiveTreeModel *model)
        : q_ptr(q), model(model) {
    }

    AdditiveTreeTransactionPrivate::~AdditiveTreeTransactionPrivate() {
    }

    // SharedData
    ATTData::ATTData() {
    }

    ATTData::ATTData(const ATTData &other) {
        operations.reserve(other.operations.size());
        for (const auto &op : other.operations)
            operations.append(cloneOperation(op));
    }

    ATTData::~ATTData() {
        qDeleteAll(operations);
    }

    ATTData::BaseOp *ATTData::cloneOperation(ATTData::BaseOp *op) {
        if (!op)
            return nullptr;

        BaseOp *res = nullptr;
        switch (op->c) {
            case PropertyChange:
                res = new PropertyChangeOp(*static_cast<PropertyChangeOp *>(op));
                break;
            case BytesSet:
                res = new BytesSetOp(*static_cast<BytesSetOp *>(op));
                break;
            case BytesTruncate:
                res = new BytesTruncateOp(*static_cast<BytesTruncateOp *>(op));
                break;
            case RowsInsert:
            case RowsRemove: {
                auto oldOp = static_cast<RowsInsertRemoveOp *>(op);
                auto newOp = new RowsInsertRemoveOp(op->c);
                newOp->id = oldOp->id;
                newOp->index = oldOp->index;
                newOp->items = qCloneAll<QVector, QVector>(oldOp->items);
                res = newOp;
                break;
            }
            case RowsMove:
                res = new RowsMoveOp(*static_cast<RowsMoveOp *>(op));
                break;
            case UniqueAdd:
            case UniqueRemove: {
                auto oldOp = static_cast<UniqueAddRemoveOp *>(op);
                auto newOp = new UniqueAddRemoveOp(op->c);
                newOp->id = oldOp->id;
                newOp->item = oldOp->item->clone();
                res = newOp;
                break;
            }
            case RootChange: {
                auto oldOp = static_cast<RootChangeOp *>(op);
                auto newOp = new RootChangeOp();
                newOp->oldRoot = oldOp->oldRoot->clone();
                newOp->newRoot = oldOp->newRoot->clone();
                res = newOp;
                break;
            }
            default:
                break;
        }
        return res;
    }

    // Command
    ATTUndoCommand::ATTUndoCommand() : done(1) {
    }

    ATTUndoCommand::~ATTUndoCommand() {
    }

    void ATTUndoCommand::undo() {
        if (!done)
            return;
        done = 0;
        if (!execute(true)) {
            setObsolete(true);
        }
    }

    void ATTUndoCommand::redo() {
        if (done)
            return;
        done = 1;
        if (!execute(false)) {
            setObsolete(true);
        }
    }

    bool ATTUndoCommand::execute(bool undo) const {
        using ATTD = ATTData;

        AdditiveTreeModel *model;
        if (!(model = data->model.data()))
            goto obsolete;

        // Execute redo
        for (const auto &op : qAsConst(data->operations)) {
            switch (op->c) {
                case ATTD::PropertyChange: {
                    auto c = static_cast<ATTD::PropertyChangeOp *>(op);
                    auto item = model->itemFromIndex(c->id);
                    if (!item)
                        goto obsolete;
                    item->setProperty(c->key, undo ? c->oldValue : c->newValue);
                    break;
                }
                case ATTD::BytesSet: {
                    auto c = static_cast<ATTD::BytesSetOp *>(op);
                    auto item = model->itemFromIndex(c->id);
                    if (!item)
                        goto obsolete;
                    if (undo) {
                        item->setBytes(c->start, c->oldBytes);
                        if (c->newBytes.size() > c->oldBytes.size())
                            item->truncateBytes(item->bytesSize() - (c->newBytes.size() - c->oldBytes.size()));
                    } else {
                        item->setBytes(c->start, c->newBytes);
                    }
                    break;
                }
                case ATTD::BytesTruncate: {
                    auto c = static_cast<ATTD::BytesTruncateOp *>(op);
                    auto item = model->itemFromIndex(c->id);
                    if (!item)
                        goto obsolete;
                    if (undo) {
                        if (c->oldBytes.isEmpty()) {
                            item->truncateBytes(c->size - c->delta);
                        } else {
                            item->setBytes(c->size, c->oldBytes);
                        }
                    } else {
                        item->truncateBytes(c->size);
                    }
                    break;
                }
                case ATTD::RowsInsert:
                case ATTD::RowsRemove: {
                    auto c = static_cast<ATTD::RowsInsertRemoveOp *>(op);
                    auto item = model->itemFromIndex(c->id);
                    if (!item)
                        goto obsolete;
                    ((c->c == ATTD::RowsRemove) ^ undo) ? item->removeRows(c->index, c->items.size())
                                                        : item->insertRows(c->index, c->items);
                    break;
                }
                case ATTD::RowsMove: {
                    auto c = static_cast<ATTD::RowsMoveOp *>(op);
                    auto item = model->itemFromIndex(c->id);
                    if (!item)
                        goto obsolete;
                    if (undo)
                        item->moveRows(c->dest - c->count, c->count, c->index);
                    else
                        item->moveRows(c->index, c->count, c->dest);
                    break;
                }
                case ATTD::UniqueAdd:
                case ATTD::UniqueRemove: {
                    auto c = static_cast<ATTD::UniqueAddRemoveOp *>(op);
                    auto item = model->itemFromIndex(c->id);
                    if (!item)
                        goto obsolete;
                    ((c->c == ATTD::UniqueRemove) ^ undo) ? item->removeUnique(c->item) : item->addUnique(c->item);
                    break;
                }
                case ATTD::RootChange: {
                    auto c = static_cast<ATTD::RootChangeOp *>(op);
                    model->setRootItem(undo ? c->oldRoot : c->newRoot);
                    break;
                }
                default:
                    break;
            }
        }

        return true;

    obsolete:
        return false;
    }
}
