#include "AdditiveTreeModel.h"
#include "AdditiveTreeModel_p.h"

#include <QMMath.h>

#include <QDataStream>
#include <QDebug>

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

    // Item
    AdditiveTreeItemPrivate::AdditiveTreeItemPrivate() {
        parent = nullptr;
        model = nullptr;
        index = 0;
    }

    AdditiveTreeItemPrivate::~AdditiveTreeItemPrivate() {
    }

    void AdditiveTreeItemPrivate::init() {
    }

    AdditiveTreeItem::AdditiveTreeItem(const QString &name) : AdditiveTreeItem(*new AdditiveTreeItemPrivate(), name) {
    }

    AdditiveTreeItem::~AdditiveTreeItem() {
        Q_D(AdditiveTreeItem);
        if (d->model && !d->model->d_func()->is_destruct)
            propagateModel(nullptr);

        qDeleteAll(d->vector);
        qDeleteAll(d->set);
    }

    QString AdditiveTreeItem::name() const {
        Q_D(const AdditiveTreeItem);
        return d->name;
    }

    AdditiveTreeItem *AdditiveTreeItem::parent() const {
        Q_D(const AdditiveTreeItem);
        return d->parent;
    }

    AdditiveTreeModel *AdditiveTreeItem::model() const {
        Q_D(const AdditiveTreeItem);
        return d->model;
    }

    int AdditiveTreeItem::index() const {
        Q_D(const AdditiveTreeItem);
        return d->model ? d->index : 0;
    }

    QVariant AdditiveTreeItem::property(const QString &key) const {
        Q_D(const AdditiveTreeItem);
        return d->properties.value(key, {});
    }

    void AdditiveTreeItem::setProperty(const QString &key, const QVariant &value) {
        Q_D(AdditiveTreeItem);
        if (d->model && d->model->d_func()->internalChange)
            return;
        d->setProperty_helper(key, value);
    }

    QVariantHash AdditiveTreeItem::properties() const {
        Q_D(const AdditiveTreeItem);
        return d->properties;
    }

    void AdditiveTreeItem::setBytes(int start, const QByteArray &bytes) {
        Q_D(AdditiveTreeItem);
        if (d->model && d->model->d_func()->internalChange)
            return;
        d->setBytes_helper(start, bytes);
    }

    void AdditiveTreeItem::truncateBytes(int size) {
        Q_D(AdditiveTreeItem);
        if (d->model && d->model->d_func()->internalChange)
            return;
        d->truncateBytes_helper(size);
    }

    QByteArray AdditiveTreeItem::bytes() const {
        Q_D(const AdditiveTreeItem);
        return d->bytes;
    }

    int AdditiveTreeItem::bytesSize() const {
        Q_D(const AdditiveTreeItem);
        return d->bytes.size();
    }

    void AdditiveTreeItem::insertRows(int index, const QVector<AdditiveTreeItem *> &items) {
        Q_D(AdditiveTreeItem);
        if (d->model && d->model->d_func()->internalChange)
            return;
        d->insertRows_helper(index, items);
    }

    void AdditiveTreeItem::moveRows(int index, int count, int dest) {
        Q_D(AdditiveTreeItem);
        if (d->model && d->model->d_func()->internalChange)
            return;
        d->moveRows_helper(index, count, dest);
    }

    void AdditiveTreeItem::removeRows(int index, int count) {
        Q_D(AdditiveTreeItem);
        if (d->model && d->model->d_func()->internalChange)
            return;
        d->removeRows_helper(index, count);
    }

    AdditiveTreeItem *AdditiveTreeItem::itemAtRow(int row) const {
        Q_D(const AdditiveTreeItem);
        return d->vector.size() > row ? d->vector.at(row) : nullptr;
    }

    void AdditiveTreeItem::addUnique(AdditiveTreeItem *item) {
        Q_D(AdditiveTreeItem);
        if (d->model && d->model->d_func()->internalChange)
            return;
        return d->addUnique_helper(item);
    }

    bool AdditiveTreeItem::containsUnique(QsApi::AdditiveTreeItem *item) {
        Q_D(const AdditiveTreeItem);
        return d->set.contains(item);
    }

    void AdditiveTreeItem::removeUnique(AdditiveTreeItem *item) {
        Q_D(AdditiveTreeItem);
        if (d->model && d->model->d_func()->internalChange)
            return;
        d->removeUnique_helper(item);
    }

    QList<AdditiveTreeItem *> AdditiveTreeItem::uniqueItems() const {
        Q_D(const AdditiveTreeItem);
        return d->set.values();
    }

    AdditiveTreeItem *AdditiveTreeItem::read(QDataStream &in) {
        char sign[4];

        in >> *((qint32 *) sign);
        if (memcmp("ITEM", sign, 4) != 0) {
            return nullptr;
        }

        QString name;
        in >> name;

        auto item = new AdditiveTreeItem(name);
        auto d = item->d_func();

        in >> d->properties;
        in >> d->bytes;

        int size;
        in >> size;
        d->vector.reserve(size);
        for (int i = 0; i < size; ++i) {
            auto child = read(in);
            if (!child)
                goto abort;
            child->d_func()->parent = item;
            d->vector.append(child);
        }

        in >> size;
        d->set.reserve(size);
        for (int i = 0; i < size; ++i) {
            auto child = read(in);
            if (!child)
                goto abort;
            child->d_func()->parent = item;
            d->set.insert(child);
        }

        return item;

    abort:
        delete item;
        return nullptr;
    }

    void AdditiveTreeItem::write(QDataStream &out) const {
        Q_D(const AdditiveTreeItem);
        // TODO: write

        out << *((qint32 *) "ITEM");
        out << d->name;
        out << d->properties;
        out << d->bytes;
        out << d->vector.size();
        for (const auto &item : d->vector) {
            item->write(out);
        }
        out << d->set.size();
        for (const auto &item : d->set) {
            item->write(out);
        }
    }

    AdditiveTreeItem *AdditiveTreeItem::clone() const {
        Q_D(const AdditiveTreeItem);
        auto item = new AdditiveTreeItem(d->name);

        auto d2 = item->d_func();
        d2->properties = d->properties;
        d2->bytes = d->bytes;

        d2->vector.reserve(d->vector.size());
        for (auto &child : d->vector) {
            auto newChild = child->clone();
            newChild->d_func()->parent = item;
            d2->vector.append(item);
        }

        d2->set.reserve(d->set.size());
        for (auto &child : d->set) {
            auto newChild = child->clone();
            newChild->d_func()->parent = item;
            d2->set.insert(item);
        }

        return item;
    }

    void AdditiveTreeItem::propagateModel(AdditiveTreeModel *model) {
        Q_D(AdditiveTreeItem);
        auto &m_model = d->model;
        if ((m_model && model) || m_model == model) {
            return;
        }

        if (m_model) {
            m_model->d_func()->removeIndex(d->index);
        } else {
            d->index = model->d_func()->addIndex(this, d->index);
        }

        m_model = model;

        for (auto item : qAsConst(d->vector))
            item->propagateModel(model);

        for (auto item : qAsConst(d->set))
            item->propagateModel(model);
    }

    AdditiveTreeItem::AdditiveTreeItem(AdditiveTreeItemPrivate &d, const QString &name) : d_ptr(&d) {
        d.q_ptr = this;
        d.name = name;

        d.init();
    }

    // Model
    AdditiveTreeModelPrivate::AdditiveTreeModelPrivate() {
        is_destruct = false;
        maxIndex = 1;
        rootItem = nullptr;
        stepIndex = 0;
        internalChange = false;
    }

    AdditiveTreeModelPrivate::~AdditiveTreeModelPrivate() {
    }

    void AdditiveTreeModelPrivate::init() {
    }

    void AdditiveTreeModelPrivate::setRootItem_helper(QsApi::AdditiveTreeItem *item) {
        Q_Q(AdditiveTreeModel);

        if (item->model() || item->parent())
            return;

        auto org = rootItem;

        // Pre-Propagate
        emit q->rootAboutToChange(org, item);

        // Do change
        indexes.clear();
        if (item)
            item->propagateModel(q);
        rootItem = item;

        // Propagate signal
        rootChanged(org, item);
    }

    int AdditiveTreeModelPrivate::addIndex(AdditiveTreeItem *item, int idx) {
        int index = idx > 0 ? (maxIndex = qMax(maxIndex, idx), idx) : maxIndex++;
        indexes.insert(index, item);
        return index;
    }

    void AdditiveTreeModelPrivate::removeIndex(int index) {
        indexes.remove(index);
    }

    void AdditiveTreeItemPrivate::setProperty_helper(const QString &key, const QVariant &value) {
        Q_Q(AdditiveTreeItem);

        QVariant oldValue;
        auto it = properties.find(key);
        if (it == properties.end()) {
            properties.insert(key, value);
        } else {
            oldValue = it.value();
            it.value() = value;
        }

        // Propagate signal
        if (model)
            model->d_func()->propertyChanged(q, key, oldValue, value);
    }

    void AdditiveTreeItemPrivate::setBytes_helper(int start, const QByteArray &bytes) {
        Q_Q(AdditiveTreeItem);

        auto len = bytes.size();
        auto oldBytes = this->bytes.mid(start, len);

        // Do change
        int newSize = start + len;
        if (newSize > this->bytes.size())
            this->bytes.resize(newSize);
        this->bytes.replace(start, len, bytes);

        // Propagate signal
        if (model)
            model->d_func()->bytesSet(q, start, oldBytes, bytes);
    }

    void AdditiveTreeItemPrivate::truncateBytes_helper(int size) {
        Q_Q(AdditiveTreeItem);

        int len = size - bytes.size();
        if (len == 0)
            return;

        auto oldBytes = bytes.mid(size);

        // Do change
        bytes.resize(size);

        // Propagate signal
        if (model)
            model->d_func()->bytesTruncated(q, size, oldBytes, len);
    }

    void AdditiveTreeItemPrivate::insertRows_helper(int index, const QVector<AdditiveTreeItem *> &items) {
        Q_Q(AdditiveTreeItem);

        QVector<AdditiveTreeItem *> validItems;
        validItems.reserve(items.size());
        for (const auto &item : items) {
            auto d2 = item->d_func();
            if (d2->model || (d2->index > 0 && !(model && model->d_func()->internalChange)))
                continue;
            validItems.append(item);
        }

        // Do change
        for (int i = 0; i < validItems.size(); ++i) {
            auto item = validItems[i];
            vector[index + i] = item;

            item->d_func()->parent = q;
            if (model)
                item->propagateModel(model);
        }
        vector.insert(vector.begin() + index, validItems.size(), nullptr);

        // Propagate signal
        if (model)
            model->d_func()->rowsInserted(q, index, validItems);
    }

    void AdditiveTreeItemPrivate::moveRows_helper(int index, int count, int dest) {
        Q_Q(AdditiveTreeItem);

        // Do change
        if (!QMMath::arrayMove(vector, index, count, dest)) {
            return;
        }

        // Propagate signal
        if (model)
            model->d_func()->rowsMoved(q, index, count, dest);
    }

    void AdditiveTreeItemPrivate::removeRows_helper(int index, int count) {
        Q_Q(AdditiveTreeItem);

        count = qMin(count, vector.size() - index);
        if (count <= 0 || count > vector.size()) {
            return;
        }

        QVector<AdditiveTreeItem *> tmp;
        tmp.resize(count);
        std::copy(vector.begin() + index, vector.begin() + index + count, tmp.begin());

        // Pre-Propagate signal
        if (model)
            emit model->rowsAboutToRemove(q, index, tmp);

        // Do change
        for (const auto &item : qAsConst(tmp)) {
            item->d_func()->parent = nullptr;
            if (model)
                item->propagateModel(nullptr);
        }
        vector.erase(vector.begin() + index, vector.begin() + index + count);

        // Propagate signal
        if (model)
            model->d_func()->rowsRemoved(q, index, tmp);
    }

    void AdditiveTreeItemPrivate::addUnique_helper(AdditiveTreeItem *item) {
        Q_Q(AdditiveTreeItem);

        auto d2 = item->d_func();
        if (d2->model || (d2->index > 0 && !(model && model->d_func()->internalChange)))
            return;

        // Do change
        item->d_func()->parent = q;
        if (model)
            item->propagateModel(model);
        set.insert(item);

        // Propagate signal
        if (model)
            model->d_func()->uniqueAdded(q, item);
    }

    void AdditiveTreeItemPrivate::removeUnique_helper(AdditiveTreeItem *item) {
        Q_Q(AdditiveTreeItem);

        if (item->parent() != q || !set.contains(item))
            return;

        // Pre-Propagate signal
        if (model)
            emit model->uniqueAboutToRemove(q, item);

        // Do change
        item->d_func()->parent = nullptr;
        if (model)
            item->propagateModel(nullptr);
        set.remove(item);

        // Propagate signal
        if (model)
            model->d_func()->uniqueRemoved(q, item);
    }

    void AdditiveTreeModelPrivate::propertyChanged(AdditiveTreeItem *item, const QString &key, const QVariant &oldValue,
                                                   const QVariant &newValue) {
        Q_Q(AdditiveTreeModel);
        if (!internalChange) {
            auto op = new PropertyChangeOp();
            op->id = item->index();
            op->key = key;
            op->oldValue = oldValue;
            op->newValue = newValue;
            push(op);
        }
        emit q->propertyChanged(item, key, oldValue, newValue);
    }

    void AdditiveTreeModelPrivate::bytesSet(AdditiveTreeItem *item, int start, const QByteArray &oldBytes,
                                            const QByteArray &newBytes) {
        Q_Q(AdditiveTreeModel);
        if (!internalChange) {
            auto op = new BytesSetOp();
            op->id = item->index();
            op->start = start;
            op->oldBytes = oldBytes;
            op->newBytes = newBytes;
            push(op);
        }
        emit q->bytesSet(item, start, oldBytes, newBytes);
    }

    void AdditiveTreeModelPrivate::bytesTruncated(AdditiveTreeItem *item, int size, const QByteArray &oldBytes,
                                                  int delta) {
        Q_Q(AdditiveTreeModel);
        if (!internalChange) {
            auto op = new BytesTruncateOp();
            op->id = item->index();
            op->size = size;
            op->oldBytes = oldBytes;
            op->delta = delta;
            push(op);
        }
        emit q->bytesTruncated(item, size, oldBytes, delta);
    }

    void AdditiveTreeModelPrivate::rowsInserted(AdditiveTreeItem *parent, int index,
                                                const QVector<AdditiveTreeItem *> &items) {
        Q_Q(AdditiveTreeModel);
        if (!internalChange) {
            auto op = new RowsInsertRemoveOp(true);
            op->id = parent->index();
            op->index = index;
            op->items = items;
            push(op);
        }
        emit q->rowsInserted(parent, index, items);
    }

    void AdditiveTreeModelPrivate::rowsMoved(AdditiveTreeItem *parent, int index, int count, int dest) {
        Q_Q(AdditiveTreeModel);
        if (!internalChange) {
            auto op = new RowsMoveOp();
            op->id = parent->index();
            op->index = index;
            op->count = count;
            op->dest = dest;
            push(op);
        }
        emit q->rowsMoved(parent, index, count, dest);
    }

    void AdditiveTreeModelPrivate::rowsRemoved(AdditiveTreeItem *parent, int index,
                                               const QVector<AdditiveTreeItem *> &items) {
        Q_Q(AdditiveTreeModel);
        if (!internalChange) {
            auto op = new RowsInsertRemoveOp(false);
            op->id = parent->index();
            op->index = index;
            op->items = items;
            push(op);
        }
        emit q->rowsRemoved(parent, index, items.size());
    }

    void AdditiveTreeModelPrivate::uniqueAdded(AdditiveTreeItem *parent, AdditiveTreeItem *item) {
        Q_Q(AdditiveTreeModel);
        if (!internalChange) {
            auto op = new UniqueAddRemoveOp(true);
            op->id = parent->index();
            op->item = item;
            push(op);
        }
        emit q->uniqueAdded(parent, item);
    }

    void AdditiveTreeModelPrivate::uniqueRemoved(AdditiveTreeItem *parent, AdditiveTreeItem *item) {
        Q_Q(AdditiveTreeModel);
        if (!internalChange) {
            auto op = new UniqueAddRemoveOp(false);
            op->id = parent->index();
            op->item = item;
            push(op);
        }
        emit q->uniqueRemoved(parent, item);
    }

    void AdditiveTreeModelPrivate::rootChanged(AdditiveTreeItem *oldRoot, AdditiveTreeItem *newRoot) {
        Q_Q(AdditiveTreeModel);
        if (!internalChange) {
            auto op = new RootChangeOp();
            op->oldRoot = oldRoot;
            op->newRoot = newRoot;
            push(op);
        }
        emit q->rootChanged();
    }

    bool AdditiveTreeModelPrivate::execute(BaseOp *op, bool undo) {
        // #define DO_CHANGE(OBJ, FUNC) OBJ->d_func()->FUNC##_helper
        auto model = q_ptr;
        switch (op->c) {
            case PropertyChange: {
                auto c = static_cast<PropertyChangeOp *>(op);
                auto item = model->itemFromIndex(c->id);
                if (!item)
                    goto obsolete;
                item->d_func()->setProperty_helper(c->key, undo ? c->oldValue : c->newValue);
                break;
            }
            case BytesSet: {
                auto c = static_cast<BytesSetOp *>(op);
                auto item = model->itemFromIndex(c->id);
                if (!item)
                    goto obsolete;
                if (undo) {
                    item->d_func()->setBytes_helper(c->start, c->oldBytes);
                    if (c->newBytes.size() > c->oldBytes.size())
                        item->d_func()->truncateBytes_helper(item->bytesSize() -
                                                             (c->newBytes.size() - c->oldBytes.size()));
                } else {
                    item->d_func()->setBytes_helper(c->start, c->newBytes);
                }
                break;
            }
            case BytesTruncate: {
                auto c = static_cast<BytesTruncateOp *>(op);
                auto item = model->itemFromIndex(c->id);
                if (!item)
                    goto obsolete;
                if (undo) {
                    if (c->oldBytes.isEmpty()) {
                        item->d_func()->truncateBytes_helper(c->size - c->delta);
                    } else {
                        item->d_func()->setBytes_helper(c->size, c->oldBytes);
                    }
                } else {
                    item->d_func()->truncateBytes_helper(c->size);
                }
                break;
            }
            case RowsInsert:
            case RowsRemove: {
                auto c = static_cast<RowsInsertRemoveOp *>(op);
                auto item = model->itemFromIndex(c->id);
                if (!item)
                    goto obsolete;
                ((c->c == RowsRemove) ^ undo) ? item->d_func()->removeRows_helper(c->index, c->items.size())
                                              : item->d_func()->insertRows_helper(c->index, c->items);
                break;
            }
            case RowsMove: {
                auto c = static_cast<RowsMoveOp *>(op);
                auto item = model->itemFromIndex(c->id);
                if (!item)
                    goto obsolete;
                if (undo)
                    item->d_func()->moveRows_helper(c->dest - c->count, c->count, c->index);
                else
                    item->d_func()->moveRows_helper(c->index, c->count, c->dest);
                break;
            }
            case UniqueAdd:
            case UniqueRemove: {
                auto c = static_cast<UniqueAddRemoveOp *>(op);
                auto item = model->itemFromIndex(c->id);
                if (!item)
                    goto obsolete;
                ((c->c == UniqueRemove) ^ undo) ? item->d_func()->removeUnique_helper(c->item)
                                                : item->d_func()->addUnique_helper(c->item);
                break;
            }
            case RootChange: {
                auto c = static_cast<RootChangeOp *>(op);
                model->d_func()->setRootItem_helper(undo ? c->oldRoot : c->newRoot);
                break;
            }
            default:
                break;
        }

        return true;

    obsolete:
        return false;
    }

    void AdditiveTreeModelPrivate::push(BaseOp *op) {
        Q_Q(AdditiveTreeModel);

        while (operations.size() > stepIndex) {
            delete operations.takeLast();
        }
        operations.append(op);
        stepIndex++;

        emit q->stepChanged(stepIndex);
    }

    AdditiveTreeModel::AdditiveTreeModel(QObject *parent) : AdditiveTreeModel(*new AdditiveTreeModelPrivate(), parent) {
    }

    AdditiveTreeModel::~AdditiveTreeModel() {
        Q_D(AdditiveTreeModel);
        d->is_destruct = true;
        delete d->rootItem;
    }

    int AdditiveTreeModel::steps() const {
        Q_D(const AdditiveTreeModel);
        return d->operations.size();
    }

    int AdditiveTreeModel::currentStep() const {
        Q_D(const AdditiveTreeModel);
        return d->stepIndex;
    }

    void AdditiveTreeModel::setCurrentStep(int step) {
        Q_D(AdditiveTreeModel);
        if (step > d->operations.size() || step == d->stepIndex || d->internalChange)
            return;

        d->internalChange = true;

        // Undo
        while (d->stepIndex > step) {
            if (!d->execute(d->operations.at(d->stepIndex - 1), true)) {
                goto out;
            }
            d->stepIndex--;
        }

        // Redo
        while (d->stepIndex < step) {
            if (!d->execute(d->operations.at(d->stepIndex), false)) {
                goto out;
            }
            d->stepIndex++;
        }

    out:
        d->internalChange = false;

        emit stepChanged(d->stepIndex);
    }

    AdditiveTreeItem *AdditiveTreeModel::itemFromIndex(int index) const {
        Q_D(const AdditiveTreeModel);
        return index > 0 ? d->indexes.value(index, nullptr) : nullptr;
    }

    AdditiveTreeItem *AdditiveTreeModel::rootItem() const {
        Q_D(const AdditiveTreeModel);
        return d->rootItem;
    }

    void AdditiveTreeModel::setRootItem(AdditiveTreeItem *item) {
        Q_D(AdditiveTreeModel);
        if (d->internalChange)
            return;
        d->setRootItem_helper(item);
    }

    AdditiveTreeModel::AdditiveTreeModel(AdditiveTreeModelPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }
}
