#ifndef PLANARITEMMODELPRIVATE_H
#define PLANARITEMMODELPRIVATE_H

#include "PlanarItemModel.h"

namespace QsApi {

    class QSSVS_API PlanarItemModelPrivate {
        Q_DECLARE_PUBLIC(PlanarItemModel)
    public:
        PlanarItemModelPrivate();
        virtual ~PlanarItemModelPrivate();

        void init();

        void itemsAboutToBeInserted(const PlanarModelIndex &parent, int layer, const QList<Point3D> &positions);
        void itemsInserted(const PlanarModelIndex &parent, int layer, const QList<Point3D> &positions);

        void itemsAboutToBeRemoved(const PlanarModelIndex &parent, int layer, const QList<Point3D> &positions);
        void itemsRemoved(const PlanarModelIndex &parent, int layer, const QList<Point3D> &positions);

        void itemsAboutToBeMoved(const PlanarModelIndex &sourceParent, int sourceLayer,
                                 const QList<Point3D> &sourcePositions, const PlanarModelIndex &destinationParent,
                                 int destinationLayer, const QList<Point3D> &destinationPositions);

        void itemMoved(const PlanarModelIndex &sourceParent, int sourceLayer, const QList<Point3D> &sourcePositions,
                       const PlanarModelIndex &destinationParent, int destinationLayer,
                       const QList<Point3D> &destinationPositions);

        void layersAboutToBeInserted(const PlanarModelIndex &parent, int first, int last);
        void layersInserted(const PlanarModelIndex &parent, int first, int last);

        void layersAboutToBeRemoved(const PlanarModelIndex &parent, int first, int last);
        void layersRemoved(const PlanarModelIndex &parent, int first, int last);

        void layersAboutToBeMoved(const PlanarModelIndex &sourceParent, int sourceStart, int sourceEnd,
                                  const PlanarModelIndex &destinationParent, int destinationLayer);
        void layersMoved(const PlanarModelIndex &parent, int start, int end, const PlanarModelIndex &destination,
                         int layer);

        bool allowMove(const PlanarModelIndex &srcParent, int srcFirst, int srcLast,
                       const PlanarModelIndex &destinationParent, int destinationChild) const;

        struct ItemChange {
            PlanarModelIndex parent;
            QList<Point3D> positions;
        };

        struct LayerChange {
            PlanarModelIndex parent;
            int first, last;
            bool needsAdjust;

            LayerChange() : parent(), first(-1), last(-1), needsAdjust(false) {
            }
            LayerChange(const PlanarModelIndex &p, int f, int l) : parent(p), first(f), last(l), needsAdjust(false) {
            }
        };

        PlanarItemModel *q_ptr;
    };

}

#endif // PLANARITEMMODELPRIVATE_H