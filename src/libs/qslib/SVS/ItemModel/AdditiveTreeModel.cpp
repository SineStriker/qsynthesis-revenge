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
            emit d->model->propertyChanged(this, key, oldValue, value);
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
            emit d->model->bytesSet(this, start, len, oldBytes, bytes);
    }

    void AdditiveTreeItem::truncateBytes(int from) {
        Q_D(AdditiveTreeItem);
        auto oldBytes = d->bytes.mid(from);

        d->bytes.resize(from);

        if (d->model)
            emit d->model->bytesTruncated(this, from, oldBytes);
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
        d->vector.insert(d->vector.begin() + index, items.size(), nullptr);
        for (int i = 0; i < items.size(); ++i) {
            auto item = items[i];
            d->vector[index + i] = item;
            if (d->model)
                item->propagateModel(d->model);
        }

        if (d->model)
            emit d->model->rowInserted(this, index, items);
    }

    void AdditiveTreeItem::moveRows(int index, int count, int dest) {
        Q_D(AdditiveTreeItem);

        if (dest >= index && dest < index + count) {
            return;
        }
        count = qMin(count, d->vector.size() - index);

        QVector<AdditiveTreeItem *> tmp;
        tmp.resize(count);
        std::copy(d->vector.begin() + index, d->vector.begin() + index + count, tmp.begin());
        // TODO: Copy items

        if (d->model)
            emit d->model->rowMoved(this, index, count, dest);
    }

    QList<AdditiveTreeItem *> AdditiveTreeItem::takeRows(int index, int count) {
        Q_D(AdditiveTreeItem);
        count = qMin(count, d->vector.size() - index);

        QVector<AdditiveTreeItem *> tmp;
        tmp.resize(count);
        std::copy(d->vector.begin() + index, d->vector.begin() + index + count, tmp.begin());

        for (const auto &item : qAsConst(tmp))
            if (d->model)
                item->propagateModel(nullptr);

        if (d->model)
            emit d->model->rowRemoved(this, index, tmp.toList());

        return tmp.toList();
    }

    void AdditiveTreeItem::removeRows(int index, int count) {
    }

    AdditiveTreeItem *AdditiveTreeItem::itemAtRow(int row) const {
        Q_D(const AdditiveTreeItem);
        return d->vector.size() > row ? d->vector.at(row) : nullptr;
    }

    void AdditiveTreeItem::addUnique(AdditiveTreeItem *item) {
        Q_D(AdditiveTreeItem);
        d->set.insert(item);

        if (d->model)
            emit d->model->uniqueAdded(this, item);
    }

    bool AdditiveTreeItem::containsUnique(QsApi::AdditiveTreeItem *item) {
        Q_D(const AdditiveTreeItem);
        return d->set.contains(item);
    }

    void AdditiveTreeItem::takeUnique(AdditiveTreeItem *item) {
        Q_D(AdditiveTreeItem);
        d->set.remove(item);

        if (d->model)
            emit d->model->uniqueRemoved(this, item);
    }

    void AdditiveTreeItem::removeUnique(AdditiveTreeItem *item) {
        Q_D(AdditiveTreeItem);
        d->set.remove(item);

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

    void AdditiveTreeItem::propagateModel(AdditiveTreeModel *model) {
        Q_D(AdditiveTreeItem);
        auto &m_model = d->model;
        if (!m_model && m_model != model) {
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
        inTransaction = false;
        dev = nullptr;
        maxIndex = 1;
        rootItem = nullptr;
    }

    AdditiveTreeModelPrivate::~AdditiveTreeModelPrivate() {
    }

    void AdditiveTreeModelPrivate::init() {
    }

    AdditiveTreeModel::AdditiveTreeModel(QObject *parent) : AdditiveTreeModel(*new AdditiveTreeModelPrivate(), parent) {
    }

    AdditiveTreeModel::~AdditiveTreeModel() {
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

    QString AdditiveTreeModel::indexFromItem(AdditiveTreeItem *item) const {
        return QString();
    }

    AdditiveTreeItem *AdditiveTreeModel::itemFromIndex(int index) const {
        return nullptr;
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
        d->rootItem = item;
    }

    AdditiveTreeModel::AdditiveTreeModel(AdditiveTreeModelPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }
}
