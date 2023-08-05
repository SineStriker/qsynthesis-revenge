#include <QPoint>
#include <QJsonObject>

#include "AceTreeMacros.h"

#include "DspxBaseEntity.h"
#include "DspxEntityUtils_p.h"

namespace Core {

    //===========================================================================
    // BusControl
    class DspxBusControlEntityExtra : public AceTreeEntityMappingExtra {
    public:
        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxBusControlEntity *>(entity);

            addPropertyNotifier("gain", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->gainChanged(value.toDouble());
            });

            addPropertyNotifier("mute", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->muteChanged(value.toBool());
            });
        }
    };
    DspxBusControlEntity::DspxBusControlEntity(QObject *parent)
        : DspxBusControlEntity(new DspxBusControlEntityExtra(), parent) {
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

    DspxBusControlEntity::DspxBusControlEntity(DspxBusControlEntityExtra *extra, QObject *parent)
        : AceTreeEntityMapping(extra, parent) {
    }
    //===========================================================================

    //===========================================================================
    // TrackControl
    class DspxTrackControlEntityExtra : public DspxBusControlEntityExtra {
    public:
        void setup(AceTreeEntity *entity) override {
            DspxBusControlEntityExtra::setup(entity);

            auto q = static_cast<DspxTrackControlEntity *>(entity);

            addPropertyNotifier("pan", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->gainChanged(value.toDouble());
            });

            addPropertyNotifier("solo", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->muteChanged(value.toBool());
            });
        }
    };

    DspxTrackControlEntity::DspxTrackControlEntity(QObject *parent)
        : DspxBusControlEntity(new DspxTrackControlEntityExtra(), parent) {
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
    class DspxIntPointEntityExtra : public AceTreeEntityNotifyExtra {
    public:
        DspxIntPointEntityExtra() : x(0), y(0) {
        }

        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxDoublePointEntity *>(entity);

            addPropertyNotifier("p", [this, q](ACE_A) {
                Q_UNUSED(oldValue)
                auto pos = value.toPoint();
                // Cache point value
                x = pos.x();
                y = pos.y();
                emit q->positionChanged(x, y);
            });
        }

        int x, y; // avoid conversion when read data
    };

    DspxIntPointEntity::DspxIntPointEntity(QObject *parent)
        : DspxIntPointEntity(new DspxIntPointEntityExtra(), parent) {
    }
    DspxIntPointEntity::~DspxIntPointEntity() {
    }
    bool DspxIntPointEntity::read(const QJsonValue &value) {
        if (!value.isObject())
            return false;
        auto obj = value.toObject();
        setPos(QPoint(obj.value("x").toInt(), obj.value("y").toInt()));
        return true;
    }
    QJsonValue DspxIntPointEntity::write() const {
        auto extra = static_cast<DspxIntPointEntityExtra *>(this->extra());
        return QJsonObject{
            {"x", extra->x},
            {"y", extra->y},
        };
    }
    int DspxIntPointEntity::x() const {
        return static_cast<DspxIntPointEntityExtra *>(extra())->x;
    }
    int DspxIntPointEntity::y() const {
        return static_cast<DspxIntPointEntityExtra *>(extra())->y;
    }
    QPoint DspxIntPointEntity::pos() const {
        auto extra = static_cast<DspxIntPointEntityExtra *>(this->extra());
        return {extra->x, extra->y};
    }
    void DspxIntPointEntity::setPos(const QPoint &pos) {
        treeItem()->setAttribute("p", pos);
    }
    void DspxIntPointEntity::doInitialize() {
        setPos({0, 0});
    }
    void DspxIntPointEntity::doSetup() {
        auto pos = treeItem()->attribute("p").toPoint();
        auto extra = static_cast<DspxIntPointEntityExtra *>(this->extra());

        // Get cache
        extra->x = pos.x();
        extra->y = pos.y();
    }
    DspxIntPointEntity::DspxIntPointEntity(DspxIntPointEntityExtra *extra, QObject *parent)
        : AceTreeEntity(extra, parent) {
    }
    //===========================================================================

    //===========================================================================
    // IntPoint List
    DspxIntPointListEntity::DspxIntPointListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
    }
    DspxIntPointListEntity::~DspxIntPointListEntity() {
    }
    void DspxIntPointListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementX<DspxIntPointEntity>);
    }
    //===========================================================================

    //===========================================================================
    // DoublePoint
    class DspxDoublePointEntityExtra : public AceTreeEntityNotifyExtra {
    public:
        DspxDoublePointEntityExtra() : x(0), y(0) {
        }

        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxDoublePointEntity *>(entity);

            addPropertyNotifier("p", [this, q](ACE_A) {
                Q_UNUSED(oldValue)
                auto pos = value.toPointF();
                // Cache point value
                x = pos.x();
                y = pos.y();
                emit static_cast<DspxDoublePointEntity *>(q)->positionChanged(x, y);
            });
        }

        double x, y; // avoid conversion when read data
    };
    DspxDoublePointEntity::DspxDoublePointEntity(QObject *parent)
        : DspxDoublePointEntity(new DspxDoublePointEntityExtra(), parent) {
    }
    DspxDoublePointEntity::~DspxDoublePointEntity() {
    }
    bool DspxDoublePointEntity::read(const QJsonValue &value) {
        if (!value.isObject())
            return false;
        auto obj = value.toObject();
        setPos(QPointF(obj.value("x").toDouble(), obj.value("y").toDouble()));
        return true;
    }
    QJsonValue DspxDoublePointEntity::write() const {
        auto extra = static_cast<DspxDoublePointEntityExtra *>(this->extra());
        return QJsonObject{
            {"x", extra->x},
            {"y", extra->y},
        };
    }
    double DspxDoublePointEntity::x() const {
        return static_cast<DspxDoublePointEntityExtra *>(this->extra())->x;
    }
    double DspxDoublePointEntity::y() const {
        return static_cast<DspxDoublePointEntityExtra *>(this->extra())->y;
    }
    QPointF DspxDoublePointEntity::pos() const {
        auto extra = static_cast<DspxDoublePointEntityExtra *>(this->extra());
        return {extra->x, extra->y};
    }
    void DspxDoublePointEntity::setPos(const QPointF &pos) {
        treeItem()->setAttribute("p", pos);
    }
    void DspxDoublePointEntity::doInitialize() {
        setPos({0, 0});
    }
    void DspxDoublePointEntity::doSetup() {
        auto pos = treeItem()->attribute("p").toPointF();
        auto extra = static_cast<DspxDoublePointEntityExtra *>(this->extra());

        // Get cache
        extra->x = pos.x();
        extra->y = pos.y();
    }
    DspxDoublePointEntity::DspxDoublePointEntity(DspxDoublePointEntityExtra *extra, QObject *parent)
        : AceTreeEntity(extra, parent) {
    }
    //===========================================================================

    //===========================================================================
    // DoublePoint List
    DspxDoublePointListEntity::DspxDoublePointListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
    }
    DspxDoublePointListEntity::~DspxDoublePointListEntity() {
    }
    void DspxDoublePointListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementX<DspxDoublePointEntity>);
    }
    //===========================================================================

    //===========================================================================
    // DspxAnchorPointEntity
    class DspxAnchorPointEntityExtra : public DspxIntPointEntityExtra {
    public:
        DspxAnchorPointEntityExtra() : i(DspxAnchorPointEntity::None) {
        }

        void setup(AceTreeEntity *entity) override {
            DspxIntPointEntityExtra::setup(entity);

            auto q = static_cast<DspxAnchorPointEntity *>(entity);
            addPropertyNotifier("i", [this, q](ACE_A) {
                Q_UNUSED(oldValue)
                // Cache interp value
                i = variantToEnum<DspxAnchorPointEntity::Interpolation>(value);
                emit q->interpChanged(i);
            });
        }

        DspxAnchorPointEntity::Interpolation i;
    };

    DspxAnchorPointEntity::DspxAnchorPointEntity(QObject *parent)
        : DspxIntPointEntity(new DspxAnchorPointEntityExtra(), parent) {
    }
    DspxAnchorPointEntity::~DspxAnchorPointEntity() {
    }
    bool DspxAnchorPointEntity::read(const QJsonValue &value) {
        if (!DspxIntPointEntity::read(value)) {
            return false;
        }
        setInterp(stringToEnum<Interpolation>(value["interp"].toString()));
        return true;
    }
    QJsonValue DspxAnchorPointEntity::write() const {
        auto obj = DspxIntPointEntity::write().toObject();
        obj.insert("interp", enumToString(static_cast<DspxAnchorPointEntityExtra *>(this->extra())->i));
        return obj;
    }
    DspxAnchorPointEntity::Interpolation DspxAnchorPointEntity::interp() const {
        return static_cast<DspxAnchorPointEntityExtra *>(this->extra())->i;
    }
    void DspxAnchorPointEntity::setInterp(DspxAnchorPointEntity::Interpolation i) {
        treeItem()->setAttribute("i", int(i));
    }
    void DspxAnchorPointEntity::doInitialize() {
        DspxIntPointEntity::doInitialize();
        setInterp(None);
    }
    void DspxAnchorPointEntity::doSetup() {
        DspxIntPointEntity::doSetup();

        // Get cache
        setInterp(variantToEnum<Interpolation>(treeItem()->attribute("i")));
    }
    //===========================================================================

    //===========================================================================
    // AnchorPoint List
    DspxAnchorPointListEntity::DspxAnchorPointListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
    }
    DspxAnchorPointListEntity::~DspxAnchorPointListEntity() {
    }
    void DspxAnchorPointListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementX<DspxAnchorPointEntity>);
    }
    //===========================================================================

} // namespace Core
