#include "AdditiveTreeModel.h"
#include "AdditiveTreeModel_p.h"

#include <QDebug>

namespace QsApi {

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

        for (auto item : qAsConst(d->vector))
            delete item;

        for (auto item : qAsConst(d->set))
            delete item;
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
        return d->index;
    }

    QVariant AdditiveTreeItem::property(const QString &key) const {
        Q_D(const AdditiveTreeItem);
        return d->properties.value(key, {});
    }

    void AdditiveTreeItem::setProperty(const QString &key, const QVariant &value) {
        Q_D(AdditiveTreeItem);
        auto oldValue = d->properties.value(key, {});
        d->properties[key] = value;

        if (d->model)
            d->model->d_func()->propertyChanged(this, key, oldValue, value);
    }

    QVariantHash AdditiveTreeItem::properties() const {
        Q_D(const AdditiveTreeItem);
        return d->properties;
    }

    void AdditiveTreeItem::setBytes(int start, int len, const QByteArray &bytes) {
        Q_D(AdditiveTreeItem);
        auto oldBytes = d->bytes.mid(start, len);

        int newSize = start + len;
        if (newSize > d->bytes.size())
            d->bytes.resize(newSize);
        d->bytes.replace(start, len, bytes);

        if (d->model)
            d->model->d_func()->bytesSet(this, start, len, oldBytes, bytes);
    }

    void AdditiveTreeItem::truncateBytes(int size) {
        Q_D(AdditiveTreeItem);
        auto oldBytes = d->bytes.mid(size);

        d->bytes.resize(size);

        if (d->model)
            d->model->d_func()->bytesTruncated(this, size, oldBytes);
    }

    QByteArray AdditiveTreeItem::bytes() const {
        Q_D(const AdditiveTreeItem);
        return d->bytes;
    }

    int AdditiveTreeItem::bytesSize() const {
        Q_D(const AdditiveTreeItem);
        return d->bytes.size();
    }

    void AdditiveTreeItem::insertRows(int index, const QList<AdditiveTreeItem *> &items) {
        Q_D(AdditiveTreeItem);
        QList<AdditiveTreeItem *> validItems;
        for (const auto &item : items) {
            if (item->model() || item->parent())
                continue;
            validItems.append(item);
        }

        d->vector.insert(d->vector.begin() + index, validItems.size(), nullptr);
        for (int i = 0; i < validItems.size(); ++i) {
            auto item = validItems[i];
            d->vector[index + i] = item;

            item->d_func()->parent = this;
            if (d->model)
                item->propagateModel(d->model);
        }

        if (d->model)
            d->model->d_func()->rowInserted(this, index, validItems);
    }

    void AdditiveTreeItem::moveRows(int index, int count, int dest) {
        Q_D(AdditiveTreeItem);
        count = qMin(count, d->vector.size() - index);
        if (count <= 0 || count > d->vector.size() || (dest >= index && dest < index + count)) {
            return;
        }

        QVector<AdditiveTreeItem *> tmp;
        tmp.resize(count);
        std::copy(d->vector.begin() + index, d->vector.begin() + index + count, tmp.begin());

        int correctDest;
        if (dest > index) {
            correctDest = dest - count;
            auto sz = correctDest - index;
            for (int i = 0; i < sz; ++i) {
                d->vector[index + i] = d->vector[index + count + i];
            }
        } else {
            correctDest = dest;
            auto sz = index - dest;
            for (int i = sz - 1; i >= 0; --i) {
                d->vector[dest + count + i] = d->vector[dest + i];
            }
        }

        std::copy(tmp.begin(), tmp.end(), d->vector.begin() + correctDest);

        if (d->model)
            d->model->d_func()->rowMoved(this, index, count, dest);
    }

    QList<AdditiveTreeItem *> AdditiveTreeItem::takeRows(int index, int count) {
        Q_D(AdditiveTreeItem);
        count = qMin(count, d->vector.size() - index);
        if (count <= 0 || count > d->vector.size()) {
            return {};
        }

        QVector<AdditiveTreeItem *> tmp;
        tmp.resize(count);
        std::copy(d->vector.begin() + index, d->vector.begin() + index + count, tmp.begin());

        for (const auto &item : qAsConst(tmp)) {
            item->d_func()->parent = nullptr;
            if (d->model)
                item->propagateModel(nullptr);
        }

        if (d->model)
            d->model->d_func()->rowRemoved(this, index, tmp.toList());

        return tmp.toList();
    }

    void AdditiveTreeItem::removeRows(int index, int count) {
        qDeleteAll(takeRows(index, count));
    }

    AdditiveTreeItem *AdditiveTreeItem::itemAtRow(int row) const {
        Q_D(const AdditiveTreeItem);
        return d->vector.size() > row ? d->vector.at(row) : nullptr;
    }

    void AdditiveTreeItem::addUnique(AdditiveTreeItem *item) {
        Q_D(AdditiveTreeItem);
        if (item->model() || item->parent())
            return;

        item->d_func()->parent = this;
        if (d->model)
            item->propagateModel(d->model);

        d->set.insert(item);

        if (d->model)
            d->model->d_func()->uniqueAdded(this, item);
    }

    bool AdditiveTreeItem::containsUnique(QsApi::AdditiveTreeItem *item) {
        Q_D(const AdditiveTreeItem);
        return d->set.contains(item);
    }

    bool AdditiveTreeItem::takeUnique(AdditiveTreeItem *item) {
        Q_D(AdditiveTreeItem);
        if (item->parent() != this)
            return false;

        item->d_func()->parent = nullptr;
        if (d->model)
            item->propagateModel(nullptr);

        d->set.remove(item);

        if (d->model)
            d->model->d_func()->uniqueRemoved(this, item);

        return true;
    }

    void AdditiveTreeItem::removeUnique(AdditiveTreeItem *item) {
        Q_D(AdditiveTreeItem);
        if (takeUnique(item))
            delete item;
    }

    QList<AdditiveTreeItem *> AdditiveTreeItem::uniqueItems() const {
        Q_D(const AdditiveTreeItem);
        return d->set.values();
    }

    void AdditiveTreeItem::read(QDataStream &in) {
        Q_D(AdditiveTreeItem);
        if (d->model) {
            qWarning() << "AdditiveTreeItem::read(): Don't read when the item is in a model";
            return;
        }

        // TODO: read
    }

    void AdditiveTreeItem::write(QDataStream &out) const {
        // TODO: write
    }

    AdditiveTreeItem *AdditiveTreeItem::clone() const {
        Q_D(const AdditiveTreeItem);
        auto item = new AdditiveTreeItem(d->name);

        auto d2 = item->d_func();
        d2->properties = d->properties;
        d2->bytes = d->bytes;

        d2->vector.reserve(d->vector.size());
        for (auto &child : d->vector)
            d2->vector.append(child->clone());

        d2->set.reserve(d->set.size());
        for (auto &child : d->set)
            d2->set.insert(child->clone());

        return item;
    }

    void AdditiveTreeItem::propagateModel(AdditiveTreeModel *model) {
        Q_D(AdditiveTreeItem);
        auto &m_model = d->model;
        if (m_model != model) {
            if (m_model) {
                m_model->d_func()->removeIndex(d->index);
            }
            d->index = model ? model->d_func()->addIndex(this) : 0;

            m_model = model;

            for (auto item : qAsConst(d->vector))
                item->propagateModel(model);

            for (auto item : qAsConst(d->set))
                item->propagateModel(model);
        }
    }

    AdditiveTreeItem::AdditiveTreeItem(AdditiveTreeItemPrivate &d, const QString &name) : d_ptr(&d) {
        d.q_ptr = this;
        d.name = name;

        d.init();
    }

    AdditiveTreeModelPrivate::AdditiveTreeModelPrivate() {
        is_destruct = false;
        inTransaction = false;
        dev = nullptr;
        maxIndex = 1;
        rootItem = nullptr;
    }

    AdditiveTreeModelPrivate::~AdditiveTreeModelPrivate() {
    }

    void AdditiveTreeModelPrivate::init() {
    }

    int AdditiveTreeModelPrivate::addIndex(AdditiveTreeItem *item) {
        int idx = maxIndex++;
        indexes.insert(idx, item);
        return idx;
    }

    void AdditiveTreeModelPrivate::removeIndex(int index) {
        indexes.remove(index);
    }

    void AdditiveTreeModelPrivate::propertyChanged(AdditiveTreeItem *item, const QString &key, const QVariant &oldValue,
                                                   const QVariant &newValue) {
        Q_Q(AdditiveTreeModel);
        emit q->propertyChanged(item, key, oldValue, newValue);
    }

    void AdditiveTreeModelPrivate::bytesSet(AdditiveTreeItem *item, int start, int len, const QByteArray &oldBytes,
                                            const QByteArray &newBytes) {
        Q_Q(AdditiveTreeModel);
        emit q->bytesSet(item, start, len, oldBytes, newBytes);
    }

    void AdditiveTreeModelPrivate::bytesTruncated(AdditiveTreeItem *item, int size, const QByteArray &oldBytes) {
        Q_Q(AdditiveTreeModel);
        emit q->bytesTruncated(item, size, oldBytes);
    }

    void AdditiveTreeModelPrivate::rowInserted(AdditiveTreeItem *parent, int index,
                                               const QList<AdditiveTreeItem *> &items) {
        Q_Q(AdditiveTreeModel);
        emit q->rowInserted(parent, index, items);
    }

    void AdditiveTreeModelPrivate::rowMoved(AdditiveTreeItem *parent, int index, int count, int dest) {
        Q_Q(AdditiveTreeModel);
        emit q->rowMoved(parent, index, count, dest);
    }

    void AdditiveTreeModelPrivate::rowRemoved(AdditiveTreeItem *parent, int index,
                                              const QList<AdditiveTreeItem *> &items) {
        Q_Q(AdditiveTreeModel);
        emit q->rowRemoved(parent, index, items);
    }

    void AdditiveTreeModelPrivate::uniqueAdded(AdditiveTreeItem *parent, AdditiveTreeItem *item) {
        Q_Q(AdditiveTreeModel);
        emit q->uniqueAdded(parent, item);
    }

    void AdditiveTreeModelPrivate::uniqueRemoved(AdditiveTreeItem *parent, AdditiveTreeItem *item) {
        Q_Q(AdditiveTreeModel);
        emit q->uniqueRemoved(parent, item);
    }

    AdditiveTreeModel::AdditiveTreeModel(QObject *parent) : AdditiveTreeModel(*new AdditiveTreeModelPrivate(), parent) {
    }

    AdditiveTreeModel::~AdditiveTreeModel() {
        Q_D(AdditiveTreeModel);
        d->is_destruct = true;
        delete d->rootItem;
    }

    void AdditiveTreeModel::record(QIODevice *dev) {
    }

    AdditiveTreeModel *AdditiveTreeModel::recover(QIODevice *dev) {
        return nullptr;
    }

    void AdditiveTreeModel::beginTransaction() {
        Q_D(AdditiveTreeModel);
        d->inTransaction = true;
    }

    void AdditiveTreeModel::endTransaction(const QVariant &desc) {
        Q_D(AdditiveTreeModel);
        d->inTransaction = false;

        // TODO: construct command
    }

    bool AdditiveTreeModel::canUndo() const {
        return false;
    }

    bool AdditiveTreeModel::canRedo() const {
        return false;
    }

    void AdditiveTreeModel::undo() {
    }

    void AdditiveTreeModel::redo() {
    }

    QVariant AdditiveTreeModel::undoDesc() const {
        return QVariant();
    }

    QVariant AdditiveTreeModel::redoDesc() const {
        return QVariant();
    }

    AdditiveTreeItem *AdditiveTreeModel::itemFromIndex(int index) const {
        Q_D(const AdditiveTreeModel);
        return d->indexes.value(index, nullptr);
    }

    AdditiveTreeItem *AdditiveTreeModel::rootItem() const {
        Q_D(const AdditiveTreeModel);
        return d->rootItem;
    }

    void AdditiveTreeModel::setRootItem(AdditiveTreeItem *item) {
        Q_D(AdditiveTreeModel);
        if (d->inTransaction) {
            qWarning() << "AdditiveTreeModel::setRootItem(): Don't set root item in transaction";
            return;
        }

        if (item->model() || item->parent())
            return;

        item->propagateModel(this);
        d->rootItem = item;
    }

    AdditiveTreeModel::AdditiveTreeModel(AdditiveTreeModelPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }
}
