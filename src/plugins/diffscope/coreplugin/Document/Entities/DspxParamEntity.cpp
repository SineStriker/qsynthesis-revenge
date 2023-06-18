#include "DspxParamEntity.h"
#include "AceTreeStandardEntity_p.h"
#include "DspxEntityUtils_p.h"

#include <QDataStream>

namespace Core {

    //===========================================================================
    // ParamCurve
    class DspxParamCurveEntityPrivate : public AceTreeEntityMappingPrivate {
        Q_DECLARE_PUBLIC(DspxParamCurveEntity)
    public:
        DspxParamCurveEntityPrivate(DspxParamCurveEntity::Type type) : type(type) {
            name = "curve";

            // Notifiers
            propertyNotifiers.insert("start", [](ACE_A) {
                ACE_Q(DspxParamCurveEntity);
                emit q->startChanged(newValue.toInt());
            });
        }
        ~DspxParamCurveEntityPrivate() = default;

        DspxParamCurveEntity::Type type;
    };
    DspxParamCurveEntity::DspxParamCurveEntity(DspxParamCurveEntity::Type type, QObject *parent)
        : DspxParamCurveEntity(*new DspxParamCurveEntityPrivate(type), parent) {
    }
    DspxParamCurveEntity::~DspxParamCurveEntity() {
    }
    DspxParamCurveEntity::Type DspxParamCurveEntity::curveType() const {
        Q_D(const DspxParamCurveEntity);
        return d->type;
    }
    int DspxParamCurveEntity::start() const {
        return attribute("start").toInt();
    }
    void DspxParamCurveEntity::setStart(int start) {
        setAttribute("start", start);
    }
    DspxParamCurveEntity::DspxParamCurveEntity(DspxParamCurveEntityPrivate &d, QObject *parent)
        : AceTreeEntityMapping(d, parent) {
    }

    //===========================================================================
    // ParamFree
    class DspxParamFreeEntityPrivate : public DspxParamCurveEntityPrivate {
    public:
        DspxParamFreeEntityPrivate() : DspxParamCurveEntityPrivate(DspxParamCurveEntity::Anchor) {
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
        return attribute("step").toInt();
    }
    DspxParamFreeDataEntity *DspxParamFreeEntity::values() const {
        Q_D(const DspxParamFreeEntity);
        return d->values;
    }
    //===========================================================================

    //===========================================================================
    // ParamFreeData
    class DspxParamFreeDataEntityPrivate : public AceTreeEntityPrivate {
        Q_DECLARE_PUBLIC(DspxParamFreeDataEntity)
    public:
        DspxParamFreeDataEntityPrivate() {
        }
        ~DspxParamFreeDataEntityPrivate() = default;

        void bytesSet(int start, const QByteArray &newBytes, const QByteArray &oldBytes) override {
        }
        void bytesTruncated(int size, const QByteArray &oldBytes, int delta) override {
        }
    };
    DspxParamFreeDataEntity::DspxParamFreeDataEntity(QObject *parent)
        : AceTreeEntity(*new DspxParamFreeDataEntityPrivate(), parent) {
        AceTreeEntityPrivate::setName(this, "values");
    }
    DspxParamFreeDataEntity::~DspxParamFreeDataEntity() {
    }
    bool DspxParamFreeDataEntity::read(const QJsonValue &value) {
        auto treeItem = AceTreeEntityPrivate::getItem(this);
        if (!value.isArray())
            return false;

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
        QDataStream stream(treeItem()->bytes());
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
    int DspxParamFreeDataEntity::size() const {
        auto treeItem = AceTreeEntityPrivate::getItem(this);
        return treeItem->bytesSize() / int(sizeof(qint16));
    }
    QList<qint16> DspxParamFreeDataEntity::mid(int index, int size) const {
        QDataStream stream(treeItem()->midBytes(index * int(sizeof(qint16)), size * int(sizeof(qint16))));
        QList<qint16> res;
        res.reserve(size);
        while (!stream.atEnd()) {
            qint16 num;
            stream >> num;
            if (stream.status() != QDataStream::Ok) {
                break;
            }
            res.append(num);
        }
        return res;
    }
    void DspxParamFreeDataEntity::append(const QList<qint16> &values) {
        auto treeItem = AceTreeEntityPrivate::getItem(this);
        QByteArray bytes;
        QDataStream stream(&bytes, QIODevice::WriteOnly);
        for (const auto &value : values)
            stream << value;
        treeItem->setBytes(treeItem->bytesSize(), bytes);
    }
    void DspxParamFreeDataEntity::replace(int index, const QList<qint16> &values) {
        auto treeItem = AceTreeEntityPrivate::getItem(this);
        QByteArray bytes;
        QDataStream stream(&bytes, QIODevice::WriteOnly);
        for (const auto &value : values)
            stream << value;
        treeItem->setBytes(index * int(sizeof(qint16)), bytes);
    }
    void DspxParamFreeDataEntity::truncate(int index) {
        auto treeItem = AceTreeEntityPrivate::getItem(this);
        treeItem->truncateBytes(index * int(sizeof(qint16)));
    }
    void DspxParamFreeDataEntity::doInitialize() {
        // Bytes
    }
    void DspxParamFreeDataEntity::doSetup() {
        // Bytes
    }
    //===========================================================================

    //===========================================================================
    // ParamAnchor
    class DspxParamAnchorEntityPrivate : public DspxParamCurveEntityPrivate {
    public:
        DspxParamAnchorEntityPrivate() : DspxParamCurveEntityPrivate(DspxParamCurveEntity::Anchor) {
            name = "anchor";
            nodes = nullptr;
            childPostAssignRefs.insert("nodes", &nodes);
        }
        ~DspxParamAnchorEntityPrivate() = default;

        DspxAnchorPointListEntity *nodes;
    };
    DspxParamAnchorEntity::DspxParamAnchorEntity(QObject *parent)
        : DspxParamCurveEntity(*new DspxParamAnchorEntityPrivate(), parent) {
    }
    DspxParamAnchorEntity::~DspxParamAnchorEntity() {
    }
    DspxAnchorPointListEntity *DspxParamAnchorEntity::nodes() const {
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
    class DspxParamInfoEntityPrivate : public AceTreeEntityMappingPrivate {
        Q_DECLARE_PUBLIC(DspxParamInfoEntity)
    public:
        DspxParamInfoEntityPrivate() {
            name = "param";
            edited = nullptr;
            childPostAssignRefs.insert("edited", &edited);

            // Notifiers
            dynamicPropertyNotifiers.insert("original", [](ACE_A) {
                ACE_Q(DspxParamInfoEntity);
                emit q->originalChanged(newValue.toJsonArray());
            });
        }
        ~DspxParamInfoEntityPrivate() = default;

        DspxParamCurveListEntity *edited;
    };
    DspxParamInfoEntity::DspxParamInfoEntity(QObject *parent)
        : DspxParamInfoEntity(*new DspxParamInfoEntityPrivate(), parent) {
    }
    DspxParamInfoEntity::~DspxParamInfoEntity() {
    }
    QJsonArray DspxParamInfoEntity::original() const {
        return attribute("original").toJsonArray();
    }
    void DspxParamInfoEntity::setOriginal(const QJsonArray &original) {
        setAttribute("original", original);
    }
    DspxParamCurveListEntity *DspxParamInfoEntity::edited() const {
        Q_D(const DspxParamInfoEntity);
        return d->edited;
    }
    DspxParamInfoEntity::DspxParamInfoEntity(DspxParamInfoEntityPrivate &d, QObject *parent)
        : AceTreeEntityMapping(d, parent) {
    }
    //===========================================================================

    //===========================================================================
    // EnvParamInfo
    class DspxEnvParamInfoEntityPrivate : public DspxParamInfoEntityPrivate {
    public:
        DspxEnvParamInfoEntityPrivate() : DspxParamInfoEntityPrivate() {
            envelope = nullptr;
            childPostAssignRefs.insert("envelope", &envelope);
        }
        ~DspxEnvParamInfoEntityPrivate() = default;

        DspxParamCurveListEntity *envelope;
    };
    DspxEnvParamInfoEntity::DspxEnvParamInfoEntity(QObject *parent)
        : DspxParamInfoEntity(*new DspxEnvParamInfoEntityPrivate(), parent) {
    }
    DspxEnvParamInfoEntity::~DspxEnvParamInfoEntity() {
    }
    DspxParamCurveListEntity *DspxEnvParamInfoEntity::envelope() const {
        Q_D(const DspxEnvParamInfoEntity);
        return d->envelope;
    }
    //===========================================================================

    //===========================================================================
    // ParamSet
    class DspxParamSetEntityPrivate : public AceTreeEntityMappingPrivate {
    public:
        DspxParamSetEntityPrivate() {
            name = "params";
            pitch = nullptr;
            childPostAssignRefs.insert("pitch", &pitch);
        }
        ~DspxParamSetEntityPrivate() = default;

        DspxParamInfoEntity *pitch;
    };
    DspxParamSetEntity::DspxParamSetEntity(QObject *parent)
        : DspxParamSetEntity(*new DspxParamSetEntityPrivate(), parent) {
    }
    DspxParamSetEntity::~DspxParamSetEntity() {
    }
    DspxParamInfoEntity *DspxParamSetEntity::pitch() const {
        Q_D(const DspxParamSetEntity);
        return d->pitch;
    }
    DspxParamSetEntity::DspxParamSetEntity(DspxParamSetEntityPrivate &d, QObject *parent)
        : AceTreeEntityMapping(d, parent) {
    }
    //===========================================================================

}