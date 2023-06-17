#include "DspxParamEntity.h"
#include "AceTreeStandardEntity_p.h"
#include "DspxEntityUtils_p.h"

#include <QDataStream>

namespace Core {

    //===========================================================================
    // ParamCurve
    class DspxParamCurveEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxParamCurveEntityPrivate(DspxParamCurveEntity::Type type, AceTreeStandardEntity::Type schemaType)
            : AceTreeStandardEntityPrivate(schemaType), type(type) {
            name = "curve";
        }
        ~DspxParamCurveEntityPrivate() = default;

        DspxParamCurveEntity::Type type;
    };
    DspxParamCurveEntity::DspxParamCurveEntity(DspxParamCurveEntity::Type type, AceTreeStandardEntity::Type schemaType,
                                               QObject *parent)
        : DspxParamCurveEntity(*new DspxParamCurveEntityPrivate(type, schemaType), parent) {
    }
    DspxParamCurveEntity::~DspxParamCurveEntity() {
    }
    DspxParamCurveEntity::Type DspxParamCurveEntity::curveType() const {
        Q_D(const DspxParamCurveEntity);
        return d->type;
    }
    int DspxParamCurveEntity::start() const {
        return property("start").toInt();
    }
    void DspxParamCurveEntity::setStart(int start) {
        setProperty("start", start);
    }
    DspxParamCurveEntity::DspxParamCurveEntity(DspxParamCurveEntityPrivate &d, QObject *parent)
        : AceTreeStandardEntity(d, parent) {
    }

    //===========================================================================
    // ParamFree
    class DspxParamFreeEntityPrivate : public DspxParamCurveEntityPrivate {
    public:
        DspxParamFreeEntityPrivate()
            : DspxParamCurveEntityPrivate(DspxParamCurveEntity::Anchor, AceTreeStandardEntity::Mapping) {
            name = "free";
            values = nullptr;
            childPostAssignRefs.insert("values", &values);
        }
        ~DspxParamFreeEntityPrivate() = default;

        DspxParamFreeDataEntity *values;
    };
    DspxParamFreeEntity::DspxParamFreeEntity(QObject *parent)
        : DspxParamCurveEntity(*new DspxParamFreeEntityPrivate(), parent) {
    }
    DspxParamFreeEntity::~DspxParamFreeEntity() {
    }
    int DspxParamFreeEntity::step() const {
        return property("step").toInt();
    }
    DspxParamFreeDataEntity *DspxParamFreeEntity::values() const {
        Q_D(const DspxParamFreeEntity);
        return d->values;
    }
    //===========================================================================

    //===========================================================================
    // ParamFreeData
    class DspxParamFreeDataEntityPrivate : public AceTreeEntityPrivate {
        Q_DECLARE_PUBLIC(DspxParamFreeEntity)
    public:
        ~DspxParamFreeDataEntityPrivate() = default;
    };
    DspxParamFreeDataEntity::DspxParamFreeDataEntity(QObject *parent)
        : AceTreeEntity(*new DspxParamFreeDataEntityPrivate(), parent) {
    }
    DspxParamFreeDataEntity::~DspxParamFreeDataEntity() {
    }
    QString DspxParamFreeDataEntity::name() const {
        return "values";
    }
    bool DspxParamFreeDataEntity::read(const QJsonValue &value) {
        Q_D(DspxParamFreeDataEntity);
        if (!value.isArray())
            return false;

        auto &treeItem = d->m_treeItem;

        // Read as int16 data
        QByteArray bytes;
        QDataStream stream(&bytes, QIODevice::WriteOnly);
        for (const auto &child : value.toArray()) {
            stream << qint16(child.isDouble() ? child.toDouble() : 0);
        }
        treeItem->setBytes(0, bytes);
        return true;
    }
    QJsonValue DspxParamFreeDataEntity::write() const {
        Q_D(const DspxParamFreeDataEntity);
        auto &treeItem = d->m_treeItem;

        QDataStream stream(treeItem->bytes());
        QJsonArray arr;
        while (!stream.atEnd()) {
            qint16 num;
            stream >> num;
            if (stream.status() != QDataStream::Ok) {
                break;
            }
            arr.append(double(num));
        }
        return arr;
    }
    //===========================================================================

    //===========================================================================
    // ParamAnchor
    class DspxParamAnchorEntityPrivate : public DspxParamCurveEntityPrivate {
    public:
        DspxParamAnchorEntityPrivate()
            : DspxParamCurveEntityPrivate(DspxParamCurveEntity::Anchor, AceTreeStandardEntity::Mapping) {
            name = "anchor";
            nodes = nullptr;
            childPostAssignRefs.insert("nodes", &nodes);
        }
        ~DspxParamAnchorEntityPrivate() = default;

        DspxIntPointListEntity *nodes;
    };
    DspxParamAnchorEntity::DspxParamAnchorEntity(QObject *parent)
        : DspxParamCurveEntity(*new DspxParamAnchorEntityPrivate(), parent) {
    }
    DspxParamAnchorEntity::~DspxParamAnchorEntity() {
    }
    DspxIntPointListEntity *DspxParamAnchorEntity::nodes() const {
        Q_D(const DspxParamAnchorEntity);
        return d->nodes;
    }
    //===========================================================================

    //===========================================================================
    // ParamCurve List
    DspxParamCurveListEntity::DspxParamCurveListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
        AceTreeStandardEntityPrivate::setName(this, "curves");
    }
    DspxParamCurveListEntity::~DspxParamCurveListEntity() {
    }
    void DspxParamCurveListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementStart<int>);
    }
    //===========================================================================

    //===========================================================================
    // ParamInfo
    class DspxParamInfoEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxParamInfoEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
            name = "param";
            edited = nullptr;
            envelope = nullptr;
            childPostAssignRefs.insert("edited", &edited);
            childPostAssignRefs.insert("envelope", &envelope);
        }
        ~DspxParamInfoEntityPrivate() = default;

        DspxParamCurveListEntity *edited;
        DspxParamCurveListEntity *envelope;
    };
    DspxParamInfoEntity::DspxParamInfoEntity(QObject *parent)
        : AceTreeStandardEntity(*new DspxParamInfoEntityPrivate(), parent) {
    }
    DspxParamInfoEntity::~DspxParamInfoEntity() {
    }
    QJsonArray DspxParamInfoEntity::original() const {
        return property("original").toJsonArray();
    }
    void DspxParamInfoEntity::setOriginal(const QJsonArray &original) {
        setProperty("original", original);
    }
    DspxParamCurveListEntity *DspxParamInfoEntity::edited() const {
        Q_D(const DspxParamInfoEntity);
        return d->edited;
    }
    DspxParamCurveListEntity *DspxParamInfoEntity::envelope() const {
        Q_D(const DspxParamInfoEntity);
        return d->envelope;
    }
    //===========================================================================

    //===========================================================================
    // ParamSet
    DspxParamSetEntity::DspxParamSetEntity(QObject *parent) : AceTreeEntityMapping(parent) {
        AceTreeStandardEntityPrivate::setName(this, "params");
    }
    DspxParamSetEntity::~DspxParamSetEntity() {
    }
    //===========================================================================

}