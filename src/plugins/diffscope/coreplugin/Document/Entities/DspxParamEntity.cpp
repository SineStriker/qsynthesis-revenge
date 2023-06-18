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
            Q_Q(DspxParamFreeDataEntity);

            int size = newBytes.size() / int(sizeof(qint16));
            auto data = reinterpret_cast<const qint16 *>(newBytes.constData());
            emit q->replaced(start / int(sizeof(qint16)), {
                                                              data,
                                                              data + size,
                                                          });
        }
        void bytesInserted(int start, const QByteArray &bytes) override {
            Q_Q(DspxParamFreeDataEntity);

            int size = bytes.size() / int(sizeof(qint16));
            auto data = reinterpret_cast<const qint16 *>(bytes.constData());
            emit q->inserted(start / int(sizeof(qint16)), {
                                                              data,
                                                              data + size,
                                                          });
        }
        void bytesRemoved(int start, const QByteArray &bytes) override {
            Q_Q(DspxParamFreeDataEntity);
            emit q->removed(start / int(sizeof(qint16)), bytes.size() / int(sizeof(qint16)));
        }
    };
    DspxParamFreeDataEntity::DspxParamFreeDataEntity(QObject *parent)
        : AceTreeEntity(*new DspxParamFreeDataEntityPrivate(), parent) {
        AceTreeEntityPrivate::setName(this, "values");
    }
    DspxParamFreeDataEntity::~DspxParamFreeDataEntity() {
    }
    bool DspxParamFreeDataEntity::read(const QJsonValue &value) {
        if (!value.isArray())
            return false;

        auto arr = value.toArray();

        QByteArray bytes;
        bytes.reserve(arr.size() * int(sizeof(qint16)));

        // Read as int16 data
        for (const auto &item : qAsConst(arr)) {
            auto num = static_cast<qint16>(item.toInt());
            bytes.append(reinterpret_cast<const char *>(&num), sizeof(num));
        }
        AceTreeEntityPrivate::getItem(this)->appendBytes(bytes);
        return true;
    }
    QJsonValue DspxParamFreeDataEntity::write() const {
        auto treeItem = this->treeItem();
        QJsonArray arr;
        auto begin = reinterpret_cast<const qint16 *>(treeItem->bytesData());
        auto end = begin + (treeItem->bytesSize() / int(sizeof(qint16)));
        for (auto it = begin; it != end; ++it) {
            arr.append(static_cast<int>(*it));
        }
        return arr;
    }
    int DspxParamFreeDataEntity::size() const {
        return treeItem()->bytesSize() / int(sizeof(qint16));
    }
    QVector<qint16> DspxParamFreeDataEntity::mid(int index, int size) const {
        auto data = reinterpret_cast<const qint16 *>(treeItem()->bytesData()) + index;
        return {data, data + size};
    }
    QVector<qint16> DspxParamFreeDataEntity::values() const {
        auto treeItem = this->treeItem();

        int size = treeItem->bytesSize() / int(sizeof(qint16));
        auto data = reinterpret_cast<const qint16 *>(treeItem->bytesData());
        return {data, data + size};
    }
    void DspxParamFreeDataEntity::replace(int index, const QVector<qint16> &values) {
        AceTreeEntityPrivate::getItem(this)->setBytes(index * int(sizeof(qint16)),
                                                      {
                                                          reinterpret_cast<const char *>(values.constData()),
                                                          values.size() * int(sizeof(qint16)),
                                                      });
    }
    void DspxParamFreeDataEntity::insert(int index, const QVector<qint16> &values) {
        AceTreeEntityPrivate::getItem(this)->insertBytes(index * int(sizeof(qint16)),
                                                         {
                                                             reinterpret_cast<const char *>(values.constData()),
                                                             values.size() * int(sizeof(qint16)),
                                                         });
    }
    void DspxParamFreeDataEntity::remove(int index, const QVector<qint16> &values) {
        AceTreeEntityPrivate::getItem(this)->removeBytes(index * int(sizeof(qint16)),
                                                         values.size() * int(sizeof(qint16)));
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
    DspxParamCurveListEntity::DspxParamCurveListEntity(QObject *parent)
        : AceTreeEntityRecordTable(parent), AceTreeEntityRecordTableHelper(this) {
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