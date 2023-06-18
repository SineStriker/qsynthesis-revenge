#include "DspxBaseEntity.h"
#include "AceTreeStandardEntity_p.h"
#include "DspxEntityUtils_p.h"

namespace Core {

    //===========================================================================
    // BusControl
    DspxBusControlEntity::DspxBusControlEntity(QObject *parent) : AceTreeEntityMapping(parent) {
        auto d = AceTreeEntityMappingPrivate::get(this);
        d->name = "control";

        // Notifiers
        d->propertyNotifiers.insert("gain", [](ACE_A) {
            ACE_Q(DspxBusControlEntity);
            emit q->gainChanged(newValue.toDouble());
        });
        d->propertyNotifiers.insert("mute", [](ACE_A) {
            ACE_Q(DspxBusControlEntity);
            emit q->muteChanged(newValue.toBool());
        });
    }
    DspxBusControlEntity::~DspxBusControlEntity() {
    }
    double DspxBusControlEntity::gain() const {
        return attribute("gain").toDouble();
    }
    void DspxBusControlEntity::setGain(double gain) {
        setAttribute("gain", gain);
    }
    bool DspxBusControlEntity::mute() const {
        return attribute("mute").toBool();
    }
    void DspxBusControlEntity::setMute(bool mute) {
        setAttribute("mute", mute);
    }
    //===========================================================================

    //===========================================================================
    // TrackControl
    DspxTrackControlEntity::DspxTrackControlEntity(QObject *parent) : DspxBusControlEntity(parent) {
        auto d = AceTreeEntityMappingPrivate::get(this);

        // Notifiers
        d->propertyNotifiers.insert("pan", [](ACE_A) {
            ACE_Q(DspxTrackControlEntity);
            emit q->gainChanged(newValue.toDouble());
        });
        d->propertyNotifiers.insert("solo", [](ACE_A) {
            ACE_Q(DspxTrackControlEntity);
            emit q->muteChanged(newValue.toBool());
        });
    }
    DspxTrackControlEntity::~DspxTrackControlEntity() {
    }
    double DspxTrackControlEntity::pan() const {
        return attribute("pan").toDouble();
    }
    void DspxTrackControlEntity::setPan(double pan) {
        setAttribute("pan", pan);
    }
    bool DspxTrackControlEntity::solo() const {
        return attribute("solo").toBool();
    }
    void DspxTrackControlEntity::setSolo(bool solo) {
        setAttribute("solo", solo);
    }
    // ==========================================================================

    //===========================================================================
    // IntPoint
    class DspxIntPointEntityPrivate : public AceTreeEntityPrivate {
        Q_DECLARE_PUBLIC(DspxIntPointEntity)
    public:
        DspxIntPointEntityPrivate() : x(0), y(0) {
            name = "point";
        }
        ~DspxIntPointEntityPrivate() = default;

        int x, y; // avoid conversion when read data

        void propertyChanged(const QString &key, const QVariant &newValue, const QVariant &oldValue) override {
            Q_Q(DspxIntPointEntity);
            if (key == "p") {
                auto pos = newValue.toPoint();
                // Cache point value
                x = pos.x();
                y = pos.y();
                emit q->positionChanged(x, y);

                qDebug() << "int point" << x << y;
            }
        }
    };
    DspxIntPointEntity::DspxIntPointEntity(QObject *parent)
        : DspxIntPointEntity(*new DspxIntPointEntityPrivate(), parent) {
    }
    DspxIntPointEntity::~DspxIntPointEntity() {
    }
    bool DspxIntPointEntity::read(const QJsonValue &value) {
        Q_D(DspxIntPointEntity);
        if (!value.isObject())
            return false;
        auto obj = value.toObject();
        setPos(QPoint(obj.value("x").toInt(), obj.value("y").toInt()));
        return true;
    }
    QJsonValue DspxIntPointEntity::write() const {
        Q_D(const DspxIntPointEntity);
        return QJsonObject{
            {"x", d->x},
            {"y", d->y},
        };
    }
    int DspxIntPointEntity::x() const {
        Q_D(const DspxIntPointEntity);
        return d->x;
    }
    int DspxIntPointEntity::y() const {
        Q_D(const DspxIntPointEntity);
        return d->y;
    }
    QPoint DspxIntPointEntity::pos() const {
        Q_D(const DspxIntPointEntity);
        return {d->x, d->y};
    }
    void DspxIntPointEntity::setPos(const QPoint &pos) {
        Q_D(DspxIntPointEntity);
        d->m_treeItem->setAttribute("p", pos);
    }
    void DspxIntPointEntity::doInitialize() {
        setPos({0, 0});
    }
    void DspxIntPointEntity::doSetup() {
        Q_D(DspxIntPointEntity);
        auto pos = d->m_treeItem->attribute("p").toPoint();

        // Get cache
        d->x = pos.x();
        d->y = pos.y();
    }
    DspxIntPointEntity::DspxIntPointEntity(Core::DspxIntPointEntityPrivate &d, QObject *parent)
        : AceTreeEntity(d, parent) {
    }
    //===========================================================================

    //===========================================================================
    // IntPoint List
    DspxIntPointListEntity::DspxIntPointListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
        AceTreeStandardEntityPrivate::setName(this, "points");
    }
    DspxIntPointListEntity::~DspxIntPointListEntity() {
    }
    void DspxIntPointListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), [](const AceTreeEntity *left, const AceTreeEntity *right) {
            return reinterpret_cast<const DspxIntPointEntity *>(left)->x() <
                   reinterpret_cast<const DspxIntPointEntity *>(right)->x();
        });
    }
    //===========================================================================

    //===========================================================================
    // DoublePoint
    class DspxDoublePointEntityPrivate : public AceTreeEntityPrivate {
        Q_DECLARE_PUBLIC(DspxDoublePointEntity)
    public:
        DspxDoublePointEntityPrivate() : x(0), y(0) {
            name = "point";
        }
        ~DspxDoublePointEntityPrivate() = default;
        double x, y; // avoid conversion when read data

        void propertyChanged(const QString &key, const QVariant &newValue, const QVariant &oldValue) override {
            Q_Q(DspxDoublePointEntity);
            if (key == "p") {
                auto pos = newValue.toPointF();
                // Cache point value
                x = pos.x();
                y = pos.y();
                emit q->positionChanged(x, y);
            }
        }
    };
    DspxDoublePointEntity::DspxDoublePointEntity(QObject *parent)
        : DspxDoublePointEntity(*new DspxDoublePointEntityPrivate(), parent) {
    }
    DspxDoublePointEntity::~DspxDoublePointEntity() {
    }
    bool DspxDoublePointEntity::read(const QJsonValue &value) {
        Q_D(DspxDoublePointEntity);
        if (!value.isObject())
            return false;
        auto obj = value.toObject();
        setPos(QPointF(obj.value("x").toDouble(), obj.value("y").toDouble()));
        return true;
    }
    QJsonValue DspxDoublePointEntity::write() const {
        Q_D(const DspxDoublePointEntity);
        return QJsonObject{
            {"x", d->x},
            {"y", d->y},
        };
    }
    double DspxDoublePointEntity::x() const {
        Q_D(const DspxDoublePointEntity);
        return d->x;
    }
    double DspxDoublePointEntity::y() const {
        Q_D(const DspxDoublePointEntity);
        return d->y;
    }
    QPointF DspxDoublePointEntity::pos() const {
        Q_D(const DspxDoublePointEntity);
        return {d->x, d->y};
    }
    void DspxDoublePointEntity::setPos(const QPointF &pos) {
        Q_D(DspxDoublePointEntity);
        d->m_treeItem->setAttribute("p", pos);
    }
    void DspxDoublePointEntity::doInitialize() {
        setPos({0, 0});
    }
    void DspxDoublePointEntity::doSetup() {
        Q_D(DspxDoublePointEntity);
        auto pos = d->m_treeItem->attribute("p").toPointF();

        // Get cache
        d->x = pos.x();
        d->y = pos.y();
    }
    DspxDoublePointEntity::DspxDoublePointEntity(Core::DspxDoublePointEntityPrivate &d, QObject *parent)
        : AceTreeEntity(d, parent) {
    }
    //===========================================================================

    //===========================================================================
    // DoublePoint List
    DspxDoublePointListEntity::DspxDoublePointListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
        AceTreeStandardEntityPrivate::setName(this, "points");
    }
    DspxDoublePointListEntity::~DspxDoublePointListEntity() {
    }
    void DspxDoublePointListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), [](const AceTreeEntity *left, const AceTreeEntity *right) {
            return reinterpret_cast<const DspxDoublePointEntity *>(left)->x() <
                   reinterpret_cast<const DspxDoublePointEntity *>(right)->x();
        });
    }
    //===========================================================================

    //===========================================================================
    // AnchorPoint
    class DspxAnchorPointEntityPrivate : public DspxIntPointEntityPrivate {
        Q_DECLARE_PUBLIC(DspxAnchorPointEntity)
    public:
        DspxAnchorPointEntityPrivate() : i(DspxAnchorPointEntity::None){};
        ~DspxAnchorPointEntityPrivate() = default;

        DspxAnchorPointEntity::Interpolation i;

        void propertyChanged(const QString &key, const QVariant &newValue, const QVariant &oldValue) override {
            DspxIntPointEntityPrivate::propertyChanged(key, newValue, oldValue);

            Q_Q(DspxAnchorPointEntity);
            if (key == "i") {
                // Cache point value
                i = static_cast<DspxAnchorPointEntity::Interpolation>(newValue.toInt());
                emit q->interpolationChanged(i);

                qDebug() << "anchor point" << i;
            }
        }
    };
    DspxAnchorPointEntity::DspxAnchorPointEntity(QObject *parent)
        : DspxIntPointEntity(*new DspxAnchorPointEntityPrivate(), parent) {
    }
    DspxAnchorPointEntity::~DspxAnchorPointEntity() {
    }
    bool DspxAnchorPointEntity::read(const QJsonValue &value) {
        Q_D(DspxAnchorPointEntity);
        if (!DspxIntPointEntity::read(value)) {
            return false;
        }
        setInterp(stringToEnum<Interpolation>(value["interp"].toString()));
        return true;
    }
    QJsonValue DspxAnchorPointEntity::write() const {
        Q_D(const DspxAnchorPointEntity);
        auto obj = DspxIntPointEntity::write().toObject();
        obj.insert("interp", enumToString(d->i));
        return obj;
    }
    DspxAnchorPointEntity::Interpolation DspxAnchorPointEntity::interp() const {
        Q_D(const DspxAnchorPointEntity);
        return d->i;
    }
    void DspxAnchorPointEntity::setInterp(DspxAnchorPointEntity::Interpolation i) {
        Q_D(DspxAnchorPointEntity);
        d->m_treeItem->setAttribute("i", int(i));
    }
    void DspxAnchorPointEntity::doInitialize() {
        DspxIntPointEntity::doInitialize();
        setInterp(None);
    }
    void DspxAnchorPointEntity::doSetup() {
        Q_D(DspxIntPointEntity);

        DspxIntPointEntity::doSetup();

        // Get cache
        setInterp(variantToEnum<Interpolation>(d->m_treeItem->attribute("i")));
    }
    //===========================================================================

    //===========================================================================
    // AnchorPoint List
    DspxAnchorPointListEntity::DspxAnchorPointListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
        AceTreeStandardEntityPrivate::setName(this, "points");
    }
    DspxAnchorPointListEntity::~DspxAnchorPointListEntity() {
    }
    void DspxAnchorPointListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), [](const AceTreeEntity *left, const AceTreeEntity *right) {
            return reinterpret_cast<const DspxAnchorPointEntity *>(left)->x() <
                   reinterpret_cast<const DspxAnchorPointEntity *>(right)->x();
        });
    }
    //===========================================================================

}