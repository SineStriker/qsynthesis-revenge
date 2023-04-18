#include "PlanarItemModel.h"
#include "PlanarItemModel_p.h"

#include <QRegularExpression>

namespace QsApi {

    PlanarItemModelPrivate::PlanarItemModelPrivate() {
    }

    PlanarItemModelPrivate::~PlanarItemModelPrivate() {
    }

    void PlanarItemModelPrivate::init() {
    }

    PlanarItemModel::PlanarItemModel(QObject *parent) : PlanarItemModel(*new PlanarItemModelPrivate(), parent) {
    }

    PlanarItemModel::~PlanarItemModel() {
    }

    int PlanarItemModel::layerCount(const PlanarModelIndex &parent) const {
        return 0;
    }

    bool PlanarItemModel::setLayerData(const PlanarModelIndex &index, int layer, const QVariant &value, int role) {
        return false;
    }

    int PlanarItemModel::childCount(int layer, const PlanarModelIndex &parent) const {
        return 0;
    }

    PlanarModelIndex PlanarItemModel::sibling(const Point3D &pos, const PlanarModelIndex &idx) const {
        return PlanarModelIndex();
    }

    bool PlanarItemModel::setData(const PlanarModelIndex &index, const QVariant &value, int role) {
        return false;
    }

    QVariant PlanarItemModel::headerData(Qt::Orientation orientation, int section, int role) const {
        Q_UNUSED(orientation);
        if (role == Qt::DisplayRole)
            return section + 1;
        return QVariant();
    }

    bool PlanarItemModel::setHeaderData(Qt::Orientation orientation, int section, const QVariant &value, int role) {
        return false;
    }

    QMap<int, QVariant> PlanarItemModel::itemData(const PlanarModelIndex &index) const {
        QMap<int, QVariant> roles;
        for (int i = 0; i < Qt::UserRole; ++i) {
            QVariant variantData = data(index, i);
            if (variantData.isValid())
                roles.insert(i, variantData);
        }
        return roles;
    }

    bool PlanarItemModel::setItemData(const PlanarModelIndex &index, const QMap<int, QVariant> &roles) {
        for (auto it = roles.begin(), e = roles.end(); it != e; ++it) {
            if (!setData(index, it.value(), it.key()))
                return false;
        }
        return true;
    }

    bool PlanarItemModel::insertLayers(int layer, int count, const PlanarModelIndex &parent) {
        return false;
    }

    bool PlanarItemModel::removeLayers(int layer, int count, const PlanarModelIndex &parent) {
        return false;
    }

    bool PlanarItemModel::moveLayers(const PlanarModelIndex &sourceParent, int sourceLayer, int count,
                                     const PlanarModelIndex &destinationParent, int destinationLayer) {
        return false;
    }

    Qt::ItemFlags PlanarItemModel::flags(const PlanarModelIndex &index) const {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }

    QList<PlanarModelIndex> PlanarItemModel::match(const PlanarModelIndex &parent, int layer,
                                                   Qt::Orientation orientation, int role, const QVariant &value,
                                                   int start, int hits, Qt::MatchFlags flags) const {
        QList<PlanarModelIndex> result;
        uint matchType = flags & 0x0F;
        Qt::CaseSensitivity cs = flags & Qt::MatchCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
        bool recurse = flags & Qt::MatchRecursive;
        bool wrap = flags & Qt::MatchWrap;
        bool allHits = (hits == -1);
        QString text;          // only convert to a string if it is needed
        QRegularExpression rx; // only create it if needed
        int from = start;
        int to = childCount(layer, parent);

        // iterates twice if wrapping
        for (int i = 0; (wrap && i < 2) || (!wrap && i < 1); ++i) {
            for (int r = from; (r < to) && (allHits || result.count() < hits); ++r) {
                auto idx = index(layer, r, parent);
                if (!idx.isValid())
                    continue;
                QVariant v = data(idx, role);
                // QVariant based matching
                if (matchType == Qt::MatchExactly) {
                    if (value == v)
                        result.append(idx);
                } else { // QString or regular expression based matching
                    if (matchType == Qt::MatchRegularExpression) {
                        if (rx.pattern().isEmpty()) {
                            if (value.userType() == QMetaType::QRegularExpression) {
                                rx = value.toRegularExpression();
                            } else {
                                rx.setPattern(value.toString());
                                if (cs == Qt::CaseInsensitive)
                                    rx.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
                            }
                        }
                    } else if (matchType == Qt::MatchWildcard) {
                        if (rx.pattern().isEmpty())
                            rx.setPattern(QRegularExpression::wildcardToRegularExpression(value.toString()));
                        if (cs == Qt::CaseInsensitive)
                            rx.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
                    } else {
                        if (text.isEmpty()) // lazy conversion
                            text = value.toString();
                    }

                    QString t = v.toString();
                    switch (matchType) {
#if QT_DEPRECATED_SINCE(5, 15)
                        QT_WARNING_PUSH
                        QT_WARNING_DISABLE_DEPRECATED
                        case Qt::MatchRegExp:
                            if (QRegExp(text, cs).exactMatch(t))
                                result.append(idx);
                            break;
                            QT_WARNING_POP
#endif
                        case Qt::MatchRegularExpression:
                            Q_FALLTHROUGH();
                        case Qt::MatchWildcard:
                            if (t.contains(rx))
                                result.append(idx);
                            break;
                        case Qt::MatchStartsWith:
                            if (t.startsWith(text, cs))
                                result.append(idx);
                            break;
                        case Qt::MatchEndsWith:
                            if (t.endsWith(text, cs))
                                result.append(idx);
                            break;
                        case Qt::MatchFixedString:
                            if (t.compare(text, cs) == 0)
                                result.append(idx);
                            break;
                        case Qt::MatchContains:
                        default:
                            if (t.contains(text, cs))
                                result.append(idx);
                    }
                }
                // if (recurse) {
                //     const auto parent = column != 0 ? idx.sibling(idx.row(), 0) : idx;
                //     if (hasChildren(parent)) { // search the hierarchy
                //         result += match(index(0, column, parent), role,
                //                         (text.isEmpty() ? value : text),
                //                         (allHits ? -1 : hits - result.count()), flags);
                //     }
                // }
            }
            // prepare for the next iteration
            from = 0;
            to = start;
        }
        return result;
    }

    PlanarModelIndex PlanarItemModel::createIndex(const Point3D &p, void *data) const {
        return PlanarModelIndex(p, data, this);
    }

    PlanarModelIndex PlanarItemModel::createIndex(const Point3D &p, quintptr id) const {
        return PlanarModelIndex(p, id, this);
    }

    void PlanarItemModel::beginInsertItems(const PlanarModelIndex &parent, int layer, const QList<Point3D> &positions) {
    }

    void PlanarItemModel::endInsertItems() {
    }

    void PlanarItemModel::beginRemoveItems(const PlanarModelIndex &parent, int layer, const QList<Point3D> &positions) {
    }

    void PlanarItemModel::endRemoveItems() {
    }

    void PlanarItemModel::beginMoveItems(const PlanarModelIndex &sourceParent, int sourceLayer,
                                         const QList<Point3D> &sourcePositions,
                                         const PlanarModelIndex &destinationParent, int destinationLayer,
                                         const QList<Point3D> destinationPositions) {
    }

    void PlanarItemModel::endMoveItems() {
    }

    void PlanarItemModel::beginInsertLayers(const QModelIndex &parent, int first, int last) {
    }

    void PlanarItemModel::endInsertLayers() {
    }

    void PlanarItemModel::beginRemoveLayers(const QModelIndex &parent, int first, int last) {
    }

    void PlanarItemModel::endRemoveLayers() {
    }

    bool PlanarItemModel::beginMoveLayers(const QModelIndex &sourceParent, int sourceFirst, int sourceLast,
                                          const QModelIndex &destinationParent, int destinationLayer) {
        return false;
    }

    void PlanarItemModel::endMoveLayers() {
    }

    void PlanarItemModel::beginResetModel() {
    }

    void PlanarItemModel::endResetModel() {
    }

    PlanarItemModel::PlanarItemModel(PlanarItemModelPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
