#ifndef SVSITEMMODEL_H
#define SVSITEMMODEL_H

#include <QAbstractItemModel>

#include "QsSVSGlobal.h"

namespace QsApi {

    class SVSItemPrivate;

    class QSSVS_API SVSItem {
        Q_DECLARE_PRIVATE(SVSItem)
    public:
        enum Type {
            Sequential,
            TimeOrdered,
            Map,
            Segmented,
        };

        explicit SVSItem(int type);
        virtual ~SVSItem();

        [[nodiscard]] int type() const;

        friend class SVSItemModel;
        friend class SVSSequentialItem;
        friend class SVSTimeOrderedItem;
        friend class SVSMapItem;
        friend class SVSSegmentedItem;

    public:
        SVSItem *parent() const;

        QModelIndex index() const;
        SVSItemModel *model() const;

        Qt::ItemFlags flags() const;
        void setFlags(Qt::ItemFlags flags);

        virtual bool canFetchMore() const;
        virtual void fetchMore();

        virtual int childCount() const;
        virtual QList<SVSItem *> children() const;
        virtual void clear();

    protected:
        void propagateModel(SVSItemModel *m);

        SVSItem(SVSItemPrivate &d, int type);

        QScopedPointer<SVSItemPrivate> d_ptr;
    };

    class SVSSequentialItemPrivate;

    class SVSSequentialItem : public SVSItem {
        Q_DECLARE_PRIVATE(SVSSequentialItem)
    public:
        explicit SVSSequentialItem(Qt::ItemFlags flags = {});
        ~SVSSequentialItem();

        friend class SVSItemModel;

    public:
        int childCount() const override;
        QList<SVSItem *> children() const override;
        void clear() override;

        void appendChild(SVSItem *item);
        void insertChild(int index, SVSItem *item);
        void insertChildren(int index, const QList<SVSItem *> &items);
        void clearChildren();

        SVSItem *child(int index);
        SVSItem *takeChild(int index);
        QList<SVSItem *> takeChildren(int index, int count);

    protected:
        SVSSequentialItem(SVSSequentialItemPrivate &d);
    };

    class SVSTimeOrderedItemPrivate;

    class SVSTimeOrderedItem : public SVSItem {
        Q_DECLARE_PRIVATE(SVSTimeOrderedItem)
    public:
        explicit SVSTimeOrderedItem(Qt::ItemFlags flags = {});
        ~SVSTimeOrderedItem();

        friend class SVSItemModel;

    public:
        int childCount() const override;
        QList<SVSItem *> children() const override;
        void clear() override;

    protected:
        SVSTimeOrderedItem(SVSTimeOrderedItemPrivate &d);
    };

    class SVSMapItemPrivate;

    class SVSMapItem : public SVSItem {
        Q_DECLARE_PRIVATE(SVSMapItem)
    public:
        explicit SVSMapItem(Qt::ItemFlags flags = {});
        ~SVSMapItem();

        friend class SVSItemModel;

    public:
        int childCount() const override;
        QList<SVSItem *> children() const override;
        void clear() override;

    protected:
        SVSMapItem(SVSMapItemPrivate &d);
    };

    class SVSSegmentedItemPrivate;

    class SVSSegmentedItem : public SVSItem {
        Q_DECLARE_PRIVATE(SVSSegmentedItem)
    public:
        explicit SVSSegmentedItem(Qt::ItemFlags flags = {});
        ~SVSSegmentedItem();

        friend class SVSItemModel;

    public:
        int childCount() const override;
        QList<SVSItem *> children() const override;
        void clear() override;

    protected:
        SVSSegmentedItem(SVSSegmentedItemPrivate &d);
    };

    class SVSItemModelPrivate;

    class QSSVS_API SVSItemModel : public QAbstractItemModel {
        Q_OBJECT
        Q_DECLARE_PRIVATE(SVSItemModel)
    public:
        explicit SVSItemModel(QObject *parent = nullptr);
        ~SVSItemModel();

    public:
        int rowCount(const QModelIndex &idx = {}) const override;
        int columnCount(const QModelIndex &idx) const override;

        QModelIndex index(int, int, const QModelIndex &idx = {}) const override;
        QModelIndex parent(const QModelIndex &idx) const override;

        SVSItem *rootItem() const;
        void setRootItem(SVSItem *item);
        SVSItem *itemFromIndex(const QModelIndex &idx) const;
        QModelIndex indexFromItem(const SVSItem *item) const;

        bool setData(const QModelIndex &idx, const QVariant &data, int role) override;
        QVariant data(const QModelIndex &idx, int role) const override;

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                           int role = Qt::EditRole) override;

        Qt::ItemFlags flags(const QModelIndex &idx) const override;
        bool hasChildren(const QModelIndex &idx) const override;

        bool canFetchMore(const QModelIndex &idx) const override;
        void fetchMore(const QModelIndex &idx) override;

    signals:
        void childAdded(SVSItem *parent, SVSItem *child, int index = -1);
        void childRemoved(SVSItem *parent, SVSItem *child, int index = -1);

        void itemDataChanged(SVSItem *item, int role, const QVariant &oldValue, const QVariant &newValue);
        void itemFlagsChanged(SVSItem *item);

    protected:
        SVSItemModel(SVSItemModelPrivate &d, QObject *parent = nullptr);

        QScopedPointer<SVSItemModelPrivate> d_ptr;

        friend class SVSItem;
        friend class SVSSequentialItem;
        friend class SVSTimeOrderedItem;
        friend class SVSMapItem;
        friend class SVSSegmentedItem;
    };

}

#endif // SVSITEMMODEL_H