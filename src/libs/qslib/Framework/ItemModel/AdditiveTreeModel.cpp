#include "AdditiveTreeModel.h"
#include "AdditiveTreeModel_p.h"

#include "AdditiveTreeTransaction.h"
#include "AdditiveTreeTransaction_p.h"

#include <QMMath.h>

#include <QDebug>

namespace QsApi {

    // Item
    AdditiveTreeItemPrivate::AdditiveTreeItemPrivate() {
        parent = nullptr;
        model = nullptr;
        index = 0;

        oldModel = nullptr;
        oldIndex = 0;
    }

    AdditiveTreeItemPrivate::~AdditiveTreeItemPrivate() {
    }

    void AdditiveTreeItemPrivate::init() {
    }

    void AdditiveTreeItemPrivate::clearOld() {
        if (oldModel) {
            oldIndex = 0;
            oldModel = nullptr;
        }
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
        return d->index;
    }

    QVariant AdditiveTreeItem::property(const QString &key) const {
        Q_D(const AdditiveTreeItem);
        return d->properties.value(key, {});
    }

    void AdditiveTreeItem::setProperty(const QString &key, const QVariant &value) {
        Q_D(AdditiveTreeItem);
        auto oldValue = d->properties.value(key, {});

        // Do change
        d->properties[key] = value;
        d->clearOld();

        // Add to transaction
        auto transaction = d->model->d_func()->transaction;
        if (transaction)
            transaction->d_func()->propertyChanged(this, key, oldValue, value);

        // Propagate signal
        if (d->model)
            emit d->model->propertyChanged(this, key, oldValue, value);
    }

    QVariantHash AdditiveTreeItem::properties() const {
        Q_D(const AdditiveTreeItem);
        return d->properties;
    }

    void AdditiveTreeItem::setBytes(int start, const QByteArray &bytes) {
        Q_D(AdditiveTreeItem);
        auto len = bytes.size();
        auto oldBytes = d->bytes.mid(start, len);

        // Do change
        int newSize = start + len;
        if (newSize > d->bytes.size())
            d->bytes.resize(newSize);
        d->bytes.replace(start, len, bytes);
        d->clearOld();

        // Add to transaction
        auto transaction = d->model->d_func()->transaction;
        if (transaction)
            transaction->d_func()->bytesSet(this, start, oldBytes, bytes);

        // Propagate signal
        if (d->model)
            emit d->model->bytesSet(this, start, oldBytes, bytes);
    }

    void AdditiveTreeItem::truncateBytes(int size) {
        Q_D(AdditiveTreeItem);
        int len = size - d->bytes.size();
        if (len == 0)
            return;

        auto oldBytes = d->bytes.mid(size);

        // Do change
        d->bytes.resize(size);
        d->clearOld();

        // Add to transaction
        auto transaction = d->model->d_func()->transaction;
        if (transaction)
            transaction->d_func()->bytesTruncated(this, size, oldBytes, len);

        // Propagate signal
        if (d->model)
            emit d->model->bytesTruncated(this, size, oldBytes, len);
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
        QVector<AdditiveTreeItem *> validItems;
        validItems.reserve(items.size());
        for (const auto &item : items) {
            if (item->model() || item->parent())
                continue;
            validItems.append(item);
        }

        // Do change
        for (int i = 0; i < validItems.size(); ++i) {
            auto item = validItems[i];
            d->vector[index + i] = item;

            item->d_func()->parent = this;
            if (d->model)
                item->propagateModel(d->model);
        }
        d->vector.insert(d->vector.begin() + index, validItems.size(), nullptr);
        d->clearOld();

        // Add to transaction
        auto transaction = d->model->d_func()->transaction;
        if (transaction)
            transaction->d_func()->rowsInserted(this, index, validItems);

        // Propagate signal
        if (d->model)
            emit d->model->rowsInserted(this, index, validItems);
    }

    void AdditiveTreeItem::moveRows(int index, int count, int dest) {
        Q_D(AdditiveTreeItem);

        auto &arr = d->vector;
        if (!QMMath::arrayMove(arr, index, count, dest)) {
            return;
        }
        d->clearOld();

        // Add to transaction
        auto transaction = d->model->d_func()->transaction;
        if (transaction)
            transaction->d_func()->rowsMoved(this, index, count, dest);

        // Propagate signal
        if (d->model)
            emit d->model->rowsMoved(this, index, count, dest);
    }

    void AdditiveTreeItem::removeRows(int index, int count) {
        Q_D(AdditiveTreeItem);
        count = qMin(count, d->vector.size() - index);
        if (count <= 0 || count > d->vector.size()) {
            return;
        }

        QVector<AdditiveTreeItem *> tmp;
        tmp.resize(count);
        std::copy(d->vector.begin() + index, d->vector.begin() + index + count, tmp.begin());

        // Pre-Propagate signal
        if (d->model)
            emit d->model->rowsAboutToRemove(this, index, tmp);

        // Do change
        for (const auto &item : qAsConst(tmp)) {
            item->d_func()->parent = nullptr;
            if (d->model)
                item->propagateModel(nullptr);
        }
        d->vector.erase(d->vector.begin() + index, d->vector.begin() + index + count);
        d->clearOld();

        // Add to transaction
        auto transaction = d->model->d_func()->transaction;
        if (transaction)
            transaction->d_func()->rowsRemoved(this, index, tmp);

        // Propagate signal
        if (d->model)
            emit d->model->rowsRemoved(this, index, tmp.size());

        // Delete if no transaction
        if (!transaction)
            qDeleteAll(tmp);
    }

    AdditiveTreeItem *AdditiveTreeItem::itemAtRow(int row) const {
        Q_D(const AdditiveTreeItem);
        return d->vector.size() > row ? d->vector.at(row) : nullptr;
    }

    void AdditiveTreeItem::addUnique(AdditiveTreeItem *item) {
        Q_D(AdditiveTreeItem);
        if (item->model() || item->parent())
            return;

        // Do change
        item->d_func()->parent = this;
        if (d->model)
            item->propagateModel(d->model);
        d->set.insert(item);
        d->clearOld();

        // Add to transaction
        auto transaction = d->model->d_func()->transaction;
        if (transaction)
            transaction->d_func()->uniqueAdded(this, item);

        // Propagate signal
        if (d->model)
            emit d->model->uniqueAdded(this, item);
    }

    bool AdditiveTreeItem::containsUnique(QsApi::AdditiveTreeItem *item) {
        Q_D(const AdditiveTreeItem);
        return d->set.contains(item);
    }

    void AdditiveTreeItem::removeUnique(AdditiveTreeItem *item) {
        Q_D(AdditiveTreeItem);
        if (item->parent() != this)
            return;

        // Pre-Propagate signal
        if (d->model)
            emit d->model->uniqueAboutToRemove(this, item);

        // Do change
        item->d_func()->parent = nullptr;
        if (d->model)
            item->propagateModel(nullptr);
        d->set.remove(item);
        d->clearOld();

        // Add to transaction
        auto transaction = d->model->d_func()->transaction;
        if (transaction)
            transaction->d_func()->uniqueRemoved(this, item);

        // Propagate signal
        if (d->model)
            emit d->model->uniqueRemoved(this, item);
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
        if ((m_model && model) || m_model == model) {
            return;
        }

        if (m_model) {
            m_model->d_func()->removeIndex(d->index);
        } else {
            d->index = model->d_func()->addIndex(this, model == d->oldModel ? d->oldIndex : -1);
            d->oldIndex = d->index;
            d->oldModel = model;
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
        dev = nullptr;
        maxIndex = 1;
        rootItem = nullptr;

        transaction = nullptr;
    }

    AdditiveTreeModelPrivate::~AdditiveTreeModelPrivate() {
    }

    void AdditiveTreeModelPrivate::init() {
    }

    int AdditiveTreeModelPrivate::addIndex(AdditiveTreeItem *item, int idx) {
        int index = idx > 0 ? idx : maxIndex++;
        indexes.insert(index, item);
        return index;
    }

    void AdditiveTreeModelPrivate::removeIndex(int index) {
        indexes.remove(index);
    }

    AdditiveTreeModel::AdditiveTreeModel(QObject *parent) : AdditiveTreeModel(*new AdditiveTreeModelPrivate(), parent) {
    }

    AdditiveTreeModel::~AdditiveTreeModel() {
        Q_D(AdditiveTreeModel);
        d->is_destruct = true;
        delete d->rootItem;
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

        if (item->model() || item->parent())
            return;

        item->propagateModel(this);

        auto org = d->rootItem;
        d->rootItem = item;

        // Add to transaction
        auto transaction = d->transaction;
        if (transaction)
            transaction->d_func()->rootChanged(org, item);

        // Propagate signal
        emit rootChanged(org, item);
    }

    AdditiveTreeTransaction *AdditiveTreeModel::transaction() const {
        Q_D(const AdditiveTreeModel);
        return d->transaction;
    }

    AdditiveTreeModel::AdditiveTreeModel(AdditiveTreeModelPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }
}
