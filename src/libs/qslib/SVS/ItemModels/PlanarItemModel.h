#ifndef PLANARITEMMODEL_H
#define PLANARITEMMODEL_H

#include <QObject>
#include <QVariant>

#include "QsSVSGlobal.h"

namespace QsApi {

    class Point3D {
    public:
        Point3D(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {
        }

        bool isValid() const {
            return x != INT_MAX && y != INT_MAX && z != INT_MAX;
        }

        bool operator==(const Point3D &other) const {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator!=(const Point3D &other) const {
            return !(*this == other);
        }

        bool operator<(const Point3D &other) const {
            return x < other.x || y < other.y || z < other.z;
        }

        static Point3D createInvalid() {
            return {INT_MAX, INT_MAX, INT_MAX};
        }

    public:
        int x, y, z;
    };

    class QSSVS_API PlanarModelIndex {
        friend class PlanarItemModel;

    public:
        inline PlanarModelIndex() noexcept : p(Point3D::createInvalid()), i(0), m(nullptr){};

        inline Point3D pos() const noexcept {
            return p;
        }
        Q_CONSTEXPR int x() const noexcept {
            return p.x;
        }
        Q_CONSTEXPR int y() const noexcept {
            return p.y;
        }
        Q_CONSTEXPR int z() const noexcept {
            return p.z;
        }
        Q_CONSTEXPR inline quintptr internalId() const noexcept {
            return i;
        }
        inline void *internalPointer() const noexcept {
            return reinterpret_cast<void *>(i);
        }
        inline PlanarModelIndex parent() const;
        inline int layer() const;
        inline PlanarModelIndex sibling(const Point3D &pos) const;
        inline PlanarModelIndex sibling(int x, int y, int z = 0) const {
            return sibling({x, y, z});
        }

        inline QVariant data(int role = Qt::DisplayRole) const;
        inline Qt::ItemFlags flags() const;

        Q_CONSTEXPR inline const PlanarItemModel *model() const noexcept {
            return m;
        }
        inline bool isValid() const noexcept {
            return p.isValid() && (m != nullptr);
        }
        inline bool operator==(const PlanarModelIndex &other) const noexcept {
            return (other.p == p) && (other.i == i) && (other.m == m);
        }
        inline bool operator!=(const PlanarModelIndex &other) const noexcept {
            return !(*this == other);
        }
        inline bool operator<(const PlanarModelIndex &other) const noexcept {
            return p < other.p || i < other.i || (i == other.i && std::less()(m, other.m));
        }

    private:
        inline PlanarModelIndex(const Point3D &p, void *ptr, const PlanarItemModel *model) noexcept
            : p(p), i(reinterpret_cast<quintptr>(ptr)), m(model) {
        }
        inline PlanarModelIndex(const Point3D &p, qintptr id, const PlanarItemModel *model) noexcept
            : p(p), i(id), m(model) {
        }

        Point3D p;
        quintptr i;
        const PlanarItemModel *m;
    };

    class PlanarItemModelPrivate;

    class QSSVS_API PlanarItemModel : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(PlanarItemModel)
    public:
        explicit PlanarItemModel(QObject *parent = nullptr);
        ~PlanarItemModel();

    public:
        virtual int layerCount(const PlanarModelIndex &parent = {}) const;
        virtual QVariant layerData(const PlanarModelIndex &index, int layer, int role = Qt::DisplayRole) const = 0;
        virtual bool setLayerData(const PlanarModelIndex &index, int layer, const QVariant &value,
                                  int role = Qt::EditRole);

        virtual int childCount(int layer, const PlanarModelIndex &parent = {}) const;
        virtual PlanarModelIndex index(int layer, int idx, const PlanarModelIndex &parent = {}) const = 0;
        virtual PlanarModelIndex index(int layer, const Point3D &pos, const PlanarModelIndex &parent = {}) const = 0;

        virtual PlanarModelIndex parent(const PlanarModelIndex &child) const = 0;
        virtual int layer(const PlanarModelIndex &child) const = 0;
        virtual PlanarModelIndex sibling(const Point3D &pos, const PlanarModelIndex &idx) const;

        virtual QVariant data(const PlanarModelIndex &index, int role = Qt::DisplayRole) const = 0;
        virtual bool setData(const PlanarModelIndex &index, const QVariant &value, int role = Qt::EditRole);

        virtual QVariant headerData(Qt::Orientation orientation, int section, int role = Qt::DisplayRole) const;
        virtual bool setHeaderData(Qt::Orientation orientation, int section, const QVariant &value,
                                   int role = Qt::EditRole);

        virtual QMap<int, QVariant> itemData(const PlanarModelIndex &index) const;
        virtual bool setItemData(const PlanarModelIndex &index, const QMap<int, QVariant> &roles);

        virtual bool insertLayers(int layer, int count, const PlanarModelIndex &parent = {});
        virtual bool removeLayers(int layer, int count, const PlanarModelIndex &parent = {});
        virtual bool moveLayers(const PlanarModelIndex &sourceParent, int sourceLayer, int count,
                                const PlanarModelIndex &destinationParent, int destinationLayer);

        inline bool insertLayer(int layer, const PlanarModelIndex &parent = {});
        inline bool removeLayer(int layer, const PlanarModelIndex &parent = {});
        inline bool moveLayer(const PlanarModelIndex &sourceParent, int sourceLayer,
                              const PlanarModelIndex &destinationParent, int destinationLayer);

        virtual Qt::ItemFlags flags(const PlanarModelIndex &index) const;
        virtual QList<PlanarModelIndex> match(const PlanarModelIndex &parent, int layer, Qt::Orientation orientation,
                                              int role, const QVariant &value, int start, int hits = 1,
                                              Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith |
                                                                                    Qt::MatchWrap)) const;

    signals:
        void dataChanged(const QList<QModelIndex> &indexes, const QVector<int> &roles);
        void layerDataChanged(int layer, const QVector<int> &roles);
        void headerDataChanged(Qt::Orientation orientation, int first, int last, const QVector<int> &roles);

        void itemsAboutToBeInserted(const PlanarModelIndex &parent, int layer, const QList<Point3D> &positions,
                                    QPrivateSignal);
        void itemsInserted(const PlanarModelIndex &parent, int layer, const QList<Point3D> &positions, QPrivateSignal);

        void itemsAboutToBeRemoved(const PlanarModelIndex &parent, int layer, const QList<Point3D> &positions,
                                   QPrivateSignal);
        void itemsRemoved(const PlanarModelIndex &parent, int layer, const QList<Point3D> &positions, QPrivateSignal);

        void itemsAboutToBeMoved(const PlanarModelIndex &sourceParent, int sourceLayer,
                                 const QList<Point3D> &sourcePositions, const PlanarModelIndex &destinationParent,
                                 int destinationLayer, const QList<Point3D> destinationPositions, QPrivateSignal);

        void itemMoved(const PlanarModelIndex &sourceParent, int sourceLayer, const QList<Point3D> &sourcePositions,
                       const PlanarModelIndex &destinationParent, int destinationLayer,
                       const QList<Point3D> destinationPositions, QPrivateSignal);

        void layersAboutToBeInserted(const QModelIndex &parent, int first, int last, QPrivateSignal);
        void layersInserted(const QModelIndex &parent, int first, int last, QPrivateSignal);

        void layersAboutToBeRemoved(const QModelIndex &parent, int first, int last, QPrivateSignal);
        void layersRemoved(const QModelIndex &parent, int first, int last, QPrivateSignal);

        void layersAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd,
                                  const QModelIndex &destinationParent, int destinationLayer, QPrivateSignal);
        void layersMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int layer,
                         QPrivateSignal);

        void modelAboutToBeReset(QPrivateSignal);
        void modelReset(QPrivateSignal);

    protected:
        inline PlanarModelIndex createIndex(const Point3D &p, void *data = nullptr) const;
        inline PlanarModelIndex createIndex(const Point3D &p, quintptr id) const;

        void beginInsertItems(const PlanarModelIndex &parent, int layer, const QList<Point3D> &positions);
        void endInsertItems();

        void beginRemoveItems(const PlanarModelIndex &parent, int layer, const QList<Point3D> &positions);
        void endRemoveItems();

        void beginMoveItems(const PlanarModelIndex &sourceParent, int sourceLayer,
                            const QList<Point3D> &sourcePositions, const PlanarModelIndex &destinationParent,
                            int destinationLayer, const QList<Point3D> destinationPositions);
        void endMoveItems();

        void beginInsertLayers(const QModelIndex &parent, int first, int last);
        void endInsertLayers();

        void beginRemoveLayers(const QModelIndex &parent, int first, int last);
        void endRemoveLayers();

        bool beginMoveLayers(const QModelIndex &sourceParent, int sourceFirst, int sourceLast,
                             const QModelIndex &destinationParent, int destinationLayer);
        void endMoveLayers();

        void beginResetModel();
        void endResetModel();

    protected:
        PlanarItemModel(PlanarItemModelPrivate &d, QObject *parent = nullptr);

        QScopedPointer<PlanarItemModelPrivate> d_ptr;
    };

    inline PlanarModelIndex PlanarModelIndex::parent() const {
        return m->parent(*this);
    }

    inline int PlanarModelIndex::layer() const {
        return m->layer(*this);
    }

    inline PlanarModelIndex PlanarModelIndex::sibling(const Point3D &pos) const {
        return m->sibling(pos, *this);
    }

    inline QVariant PlanarModelIndex::data(int role) const {
        return m->data(*this, role);
    }

    inline Qt::ItemFlags PlanarModelIndex::flags() const {
        return m->flags(*this);
    }

    inline bool PlanarItemModel::insertLayer(int layer, const PlanarModelIndex &parent) {
        return insertLayers(layer, 1, parent);
    }
    inline bool PlanarItemModel::removeLayer(int layer, const PlanarModelIndex &parent) {
        return removeLayers(layer, 1, parent);
    }
    inline bool PlanarItemModel::moveLayer(const PlanarModelIndex &sourceParent, int sourceLayer,
                                           const PlanarModelIndex &destinationParent, int destinationLayer) {
        return moveLayers(sourceParent, sourceLayer, 1, destinationParent, destinationLayer);
    }

}

inline uint qHash(const QsApi::PlanarModelIndex &index) noexcept {
    return uint((index.x() << 12) + (index.y() << 4) + index.z() + index.internalId());
}


#endif // PLANARITEMMODEL_H