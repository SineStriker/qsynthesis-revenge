#include "SVSItemModel.h"
#include "SVSItemModel_p.h"

namespace QsApi {

    // SVSItem
    SVSItemPrivate::SVSItemPrivate() {
        parent = nullptr;
        model = nullptr;
        flags = {};
    }

    SVSItemPrivate::~SVSItemPrivate() {
    }

    void SVSItemPrivate::init() {
    }

    SVSItem::SVSItem(int type) : SVSItem(*new SVSItemPrivate(), type) {
    }

    SVSItem::~SVSItem() {
    }

    int SVSItem::type() const {
        Q_D(const SVSItem);
        return d->type;
    }

    SVSItem *SVSItem::parent() const {
        Q_D(const SVSItem);
        return d->parent;
    }

    QModelIndex SVSItem::index() const {
        Q_D(const SVSItem);
        if (d->model)
            return d->model->indexFromItem(this);
        return {};
    }

    SVSItemModel *SVSItem::model() const {
        Q_D(const SVSItem);
        return d->model;
    }

    Qt::ItemFlags SVSItem::flags() const {
        Q_D(const SVSItem);
        return d->flags;
    }

    void SVSItem::setFlags(Qt::ItemFlags flags) {
        Q_D(SVSItem);
        d->flags = flags;
        if (d->model)
            emit d->model->itemFlagsChanged(this);
    }

    bool SVSItem::canFetchMore() const {
        return false;
    }

    void SVSItem::fetchMore() {
    }

    int SVSItem::childCount() const {
        return children().size();
    }

    QList<SVSItem *> SVSItem::children() const {
        return {};
    }

    void SVSItem::clear() {
    }

    void SVSItem::propagateModel(SVSItemModel *m) {
        Q_D(SVSItem);
        auto &m_model = d->model;
        if (!m_model && m_model != m) {
            m_model = m;
            foreach (auto item, children())
                item->propagateModel(m);
        }
    }

    SVSItem::SVSItem(SVSItemPrivate &d, int type) : d_ptr(&d) {
        d.q_ptr = this;
        d.type = type;

        d.init();
    }

    // SVSSequentialItem
    SVSSequentialItemPrivate::SVSSequentialItemPrivate() {
    }

    SVSSequentialItemPrivate::~SVSSequentialItemPrivate() {
    }

    void SVSSequentialItemPrivate::init() {
    }

    SVSSequentialItem::SVSSequentialItem(Qt::ItemFlags flags) : SVSSequentialItem(*new SVSSequentialItemPrivate()) {
        setFlags(flags);
    }

    SVSSequentialItem::~SVSSequentialItem() {
    }

    int SVSSequentialItem::childCount() const {
        Q_D(const SVSSequentialItem);
        return d->children.size();
    }

    QList<SVSItem *> SVSSequentialItem::children() const {
        Q_D(const SVSSequentialItem);
        return d->children.toList();
    }

    void SVSSequentialItem::clear() {
        clearChildren();
    }

    void SVSSequentialItem::appendChild(SVSItem *item) {
        Q_D(SVSSequentialItem);
        insertChild(d->children.size(), item);
    }

    void SVSSequentialItem::insertChild(int index, SVSItem *item) {
        return insertChildren(index, {item});
    }

    void SVSSequentialItem::insertChildren(int index, const QList<SVSItem *> &items) {
        Q_D(SVSSequentialItem);
        if (index < 0 || index > d->children.size())
            return;

        auto &model = d->model;

        auto cl = [&]() {
            d->children.insert(d->children.begin() + index, items.size(), nullptr);
            for (int i = 0; i < items.size(); ++i) {
                d->children[i + index] = items.at(i);
            }
        };

        if (model) {
            QModelIndex idx = this->index();
            model->beginInsertRows(idx, index, index + items.size() - 1);

            for (const auto &item : items) {
                // emit model->childAboutToAdd(this, item, index);
                item->propagateModel(model);
                item->d_func()->parent = this;
            }

            cl();

            for (const auto &item : items)
                emit model->childAdded(this, item, index);

            model->endInsertRows();
        } else {
            for (const auto &item : items)
                item->d_func()->parent = this;

            cl();
        }
    }

    void SVSSequentialItem::clearChildren() {
        Q_D(SVSSequentialItem);
        qDeleteAll(takeChildren(0, d->children.size()));
    }

    SVSItem *SVSSequentialItem::child(int index) {
        Q_D(const SVSSequentialItem);

        if (index < 0 || index > d->children.size())
            return nullptr;
        return d->children.at(index);
    }

    SVSItem *SVSSequentialItem::takeChild(int index) {
        Q_D(SVSSequentialItem);
        auto res = takeChildren(index, 1);
        return res.isEmpty() ? nullptr : res.front();
    }

    QList<SVSItem *> SVSSequentialItem::takeChildren(int index, int count) {
        Q_D(SVSSequentialItem);

        if (index < 0 || index >= d->children.size())
            return {};

        int last = qMin(index + count - 1, d->children.size() - 1);

        auto cl = [&]() {
            d->children.erase(d->children.begin() + index, d->children.begin() + last); //
        };

        auto &model = d->model;
        QList<SVSItem *> res;
        if (model) {
            QModelIndex idx = this->index();
            model->beginRemoveRows(idx, index, last);

            for (int i = index; i < last; ++i) {
                auto item = d->children.at(i);
                // emit model->childAboutToRemove(this, item, i);
                res.append(item);

                item->propagateModel(nullptr);
                item->d_func()->parent = nullptr;
            }

            cl();

            for (int i = index; i < last; ++i) {
                emit model->childRemoved(this, res.at(i - index), i);
            }

            model->endRemoveRows();
        } else {
            for (int i = index; i < last; ++i) {
                auto item = d->children.at(i);
                res.append(item);

                item->d_func()->parent = nullptr;
            }

            cl();
        }

        return res;
    }

    SVSSequentialItem::SVSSequentialItem(SVSSequentialItemPrivate &d) : SVSItem(d, SVSItem::Sequential) {
        d.init();
    }

    // SVSTimeOrderedItem
    SVSTimeOrderedItemPrivate::SVSTimeOrderedItemPrivate() {
    }

    SVSTimeOrderedItemPrivate::~SVSTimeOrderedItemPrivate() {
    }

    void SVSTimeOrderedItemPrivate::init() {
    }

    SVSTimeOrderedItem::SVSTimeOrderedItem(Qt::ItemFlags flags) : SVSTimeOrderedItem(*new SVSTimeOrderedItemPrivate()) {
        setFlags(flags);
    }

    SVSTimeOrderedItem::~SVSTimeOrderedItem() {
    }

    int SVSTimeOrderedItem::childCount() const {
        // TODO
        return 0;
    }

    QList<SVSItem *> SVSTimeOrderedItem::children() const {
        // TODO
        return {};
    }

    void SVSTimeOrderedItem::clear() {
        SVSItem::clear();
    }

    SVSTimeOrderedItem::SVSTimeOrderedItem(SVSTimeOrderedItemPrivate &d) : SVSItem(d, SVSItem::TimeOrdered) {
        d.init();
    }

    // SVSMapItem
    SVSMapItemPrivate::SVSMapItemPrivate() {
    }

    SVSMapItemPrivate::~SVSMapItemPrivate() {
    }

    void SVSMapItemPrivate::init() {
    }

    SVSMapItem::SVSMapItem(Qt::ItemFlags flags) : SVSMapItem(*new SVSMapItemPrivate()) {
        setFlags(flags);
    }

    SVSMapItem::~SVSMapItem() {
    }

    int SVSMapItem::childCount() const {
        // TODO
        return 0;
    }

    QList<SVSItem *> SVSMapItem::children() const {
        // TODO
        return {};
    }

    void SVSMapItem::clear() {
        SVSItem::clear();
    }

    SVSMapItem::SVSMapItem(SVSMapItemPrivate &d) : SVSItem(d, SVSItem::Map) {
        d.init();
    }

    // SVSSegmentedItem
    SVSSegmentedItemPrivate::SVSSegmentedItemPrivate() {
    }

    SVSSegmentedItemPrivate::~SVSSegmentedItemPrivate() {
    }

    void SVSSegmentedItemPrivate::init() {
    }

    SVSSegmentedItem::SVSSegmentedItem(Qt::ItemFlags flags) : SVSSegmentedItem(*new SVSSegmentedItemPrivate()) {
        setFlags(flags);
    }

    SVSSegmentedItem::~SVSSegmentedItem() {
    }

    int SVSSegmentedItem::childCount() const {
        // TODO
        return 0;
    }

    QList<SVSItem *> SVSSegmentedItem::children() const {
        // TODO
        return {};
    }

    void SVSSegmentedItem::clear() {
        SVSItem::clear();
    }

    SVSSegmentedItem::SVSSegmentedItem(SVSSegmentedItemPrivate &d) : SVSItem(d, SVSItem::Segmented) {
        d.init();
    }

    // SVSItemModel
    SVSItemModelPrivate::SVSItemModelPrivate() {
    }

    SVSItemModelPrivate::~SVSItemModelPrivate() {
    }

    void SVSItemModelPrivate::init() {
    }

    SVSItemModel::SVSItemModel(QObject *parent) : SVSItemModel(*new SVSItemModelPrivate(), parent) {
    }

    SVSItemModel::~SVSItemModel() {
    }

    SVSItemModel::SVSItemModel(SVSItemModelPrivate &d, QObject *parent) : QAbstractItemModel(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

    int SVSItemModel::rowCount(const QModelIndex &idx) const {
        return 0;
    }

    int SVSItemModel::columnCount(const QModelIndex &idx) const {
        return 0;
    }

    QModelIndex SVSItemModel::index(int, int, const QModelIndex &idx) const {
        return QModelIndex();
    }

    QModelIndex SVSItemModel::parent(const QModelIndex &idx) const {
        return QModelIndex();
    }

    SVSItem *SVSItemModel::rootItem() const {
        return nullptr;
    }

    void SVSItemModel::setRootItem(SVSItem *item) {
    }

    SVSItem *SVSItemModel::itemFromIndex(const QModelIndex &idx) const {
        return nullptr;
    }

    QModelIndex SVSItemModel::indexFromItem(const SVSItem *item) const {
        return QModelIndex();
    }

    bool SVSItemModel::setData(const QModelIndex &idx, const QVariant &data, int role) {
        return QAbstractItemModel::setData(idx, data, role);
    }

    QVariant SVSItemModel::data(const QModelIndex &idx, int role) const {
        return QVariant();
    }

    QVariant SVSItemModel::headerData(int section, Qt::Orientation orientation, int role) const {
        return QAbstractItemModel::headerData(section, orientation, role);
    }

    bool SVSItemModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) {
        return QAbstractItemModel::setHeaderData(section, orientation, value, role);
    }

    Qt::ItemFlags SVSItemModel::flags(const QModelIndex &idx) const {
        return QAbstractItemModel::flags(idx);
    }

    bool SVSItemModel::hasChildren(const QModelIndex &idx) const {
        return QAbstractItemModel::hasChildren(idx);
    }

    bool SVSItemModel::canFetchMore(const QModelIndex &idx) const {
        return QAbstractItemModel::canFetchMore(idx);
    }

    void SVSItemModel::fetchMore(const QModelIndex &idx) {
        QAbstractItemModel::fetchMore(idx);
    }

}
