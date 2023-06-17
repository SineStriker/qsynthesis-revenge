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
    int DspxParamFreeDataEntity::size() const {
        Q_D(const DspxParamFreeDataEntity);
        return d->m_treeItem->bytesSize() / int(sizeof(qint16));
    }
    QList<qint16> DspxParamFreeDataEntity::mid(int index, int size) const {
        Q_D(const DspxParamFreeDataEntity);
        QDataStream stream(d->m_treeItem->midBytes(index * int(sizeof(qint16)), size * int(sizeof(qint16))));
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
        Q_D(DspxParamFreeDataEntity);
        QByteArray bytes;
        QDataStream stream(&bytes, QIODevice::WriteOnly);
        for (const auto &value : values)
            stream << value;
        d->m_treeItem->setBytes(d->m_treeItem->bytesSize(), bytes);
    }
    void DspxParamFreeDataEntity::replace(int index, const QList<qint16> &values) {
        Q_D(DspxParamFreeDataEntity);
        QByteArray bytes;
        QDataStream stream(&bytes, QIODevice::WriteOnly);
        for (const auto &value : values)
            stream << value;
        d->m_treeItem->setBytes(index * int(sizeof(qint16)), bytes);
    }
    void DspxParamFreeDataEntity::truncate(int index) {
        Q_D(DspxParamFreeDataEntity);
        d->m_treeItem->truncateBytes(index * int(sizeof(qint16)));
    }
    bool DspxParamFreeDataEntity::read(const QJsonValue &value) {
        Q_D(DspxParamFreeDataEntity);
        if (!value.isArray())
            return false;

        // Read as int16 data
        QByteArray bytes;
        QDataStream stream(&bytes, QIODevice::WriteOnly);
        for (const auto &child : value.toArray()) {
            stream << qint16(child.isDouble() ? child.toDouble() : 0);
        }
        d->m_treeItem->setBytes(0, bytes);
        return true;
    }
    QJsonValue DspxParamFreeDataEntity::write() const {
        Q_D(const DspxParamFreeDataEntity);
        QDataStream stream(d->m_treeItem->bytes());
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
    class DspxParamInfoEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxParamInfoEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
            name = "param";
            edited = nullptr;
            childPostAssignRefs.insert("edited", &edited);
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
        return property("original").toJsonArray();
    }
    void DspxParamInfoEntity::setOriginal(const QJsonArray &original) {
        setProperty("original", original);
    }
    DspxParamCurveListEntity *DspxParamInfoEntity::edited() const {
        Q_D(const DspxParamInfoEntity);
        return d->edited;
    }
    DspxParamInfoEntity::DspxParamInfoEntity(DspxParamInfoEntityPrivate &d, QObject *parent)
        : AceTreeStandardEntity(d, parent) {
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
    class DspxParamSetEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxParamSetEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
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
        : AceTreeStandardEntity(d, parent) {
    }
    //===========================================================================

}