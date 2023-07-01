#include "DspxParamEntity.h"
#include "DspxEntityUtils_p.h"

#include <QDataStream>

#include "AceTreeMacros.h"

namespace Core {

    //===========================================================================
    // ParamCurve
    class DspxParamCurveEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxParamCurveEntityExtra(DspxParamCurveEntity::Type type) : type(type) {
        }

        DspxParamCurveEntity::Type type;
    };
    DspxParamCurveEntity::~DspxParamCurveEntity() {
    }
    DspxParamCurveEntity::Type DspxParamCurveEntity::curveType() const {
        return static_cast<DspxParamCurveEntityExtra *>(extra())->type;
    }
    int DspxParamCurveEntity::start() const {
        return attribute("start").toInt();
    }
    void DspxParamCurveEntity::setStart(int start) {
        setAttribute("start", start);
    }
    DspxParamCurveEntity::DspxParamCurveEntity(DspxParamCurveEntityExtra *extra, QObject *parent)
        : AceTreeEntityMapping(extra, parent) {
    }

    //===========================================================================
    // ParamFree
    class DspxParamFreeEntityExtra : public DspxParamCurveEntityExtra {
    public:
        DspxParamFreeEntityExtra() : DspxParamCurveEntityExtra(DspxParamCurveEntity::Anchor), values(nullptr) {
        }

        void setup(AceTreeEntity *entity) override {
            Q_UNUSED(entity);
            addChildPointer("values", values);
        }

        DspxParamFreeDataEntity *values;
    };
    DspxParamFreeEntity::DspxParamFreeEntity(QObject *parent)
        : DspxParamCurveEntity(new DspxParamFreeEntityExtra(), parent) {
    }
    DspxParamFreeEntity::~DspxParamFreeEntity() {
    }
    int DspxParamFreeEntity::step() const {
        return attribute("step").toInt();
    }
    DspxParamFreeDataEntity *DspxParamFreeEntity::values() const {
        return static_cast<DspxParamFreeEntityExtra *>(extra())->values;
    }
    //===========================================================================

    //===========================================================================
    // ParamFreeData
    DspxParamFreeDataEntity::DspxParamFreeDataEntity(QObject *parent) : AceTreeNumbersEntity(parent) {
    }
    DspxParamFreeDataEntity::~DspxParamFreeDataEntity() {
    }
    //===========================================================================

    //===========================================================================
    // ParamAnchor
    class DspxParamAnchorEntityExtra : public DspxParamCurveEntityExtra {
    public:
        DspxParamAnchorEntityExtra() : DspxParamCurveEntityExtra(DspxParamCurveEntity::Anchor), nodes(nullptr) {
        }

        void setup(AceTreeEntity *entity) override {
            Q_UNUSED(entity);
            addChildPointer("nodes", nodes);
        }

        DspxAnchorPointListEntity *nodes;
    };
    DspxParamAnchorEntity::DspxParamAnchorEntity(QObject *parent)
        : DspxParamCurveEntity(new DspxParamAnchorEntityExtra(), parent) {
    }
    DspxParamAnchorEntity::~DspxParamAnchorEntity() {
    }
    DspxAnchorPointListEntity *DspxParamAnchorEntity::nodes() const {
        return static_cast<DspxParamAnchorEntityExtra *>(extra())->nodes;
    }
    //===========================================================================

    //===========================================================================
    // ParamCurve List
    DspxParamCurveListEntity::DspxParamCurveListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
    }
    DspxParamCurveListEntity::~DspxParamCurveListEntity() {
    }
    void DspxParamCurveListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementStart<DspxParamCurveEntity>);
    }
    //===========================================================================

    //===========================================================================
    // ParamInfo
    class DspxParamInfoEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxParamInfoEntityExtra() : edited(nullptr) {
        }

        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxParamInfoEntity *>(entity);

            addChildPointer("edited", edited);

            addDynamicDataNotifier("original", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->originalChanged(value.toJsonArray());
            });
        }

        DspxParamCurveListEntity *edited;
    };
    DspxParamInfoEntity::DspxParamInfoEntity(QObject *parent)
        : DspxParamInfoEntity(new DspxParamInfoEntityExtra(), parent) {
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
        return static_cast<DspxParamInfoEntityExtra *>(extra())->edited;
    }
    DspxParamInfoEntity::DspxParamInfoEntity(DspxParamInfoEntityExtra *extra, QObject *parent)
        : AceTreeEntityMapping(extra, parent) {
    }
    //===========================================================================

    //===========================================================================
    // EnvParamInfo
    class DspxEnvParamInfoEntityExtra : public DspxParamInfoEntityExtra {
    public:
        DspxEnvParamInfoEntityExtra() : envelope(nullptr) {
        }

        void setup(AceTreeEntity *entity) override {
            DspxParamInfoEntityExtra::setup(entity);

            addChildPointer("envelope", envelope);
        }

        DspxParamCurveListEntity *envelope;
    };
    DspxEnvParamInfoEntity::DspxEnvParamInfoEntity(QObject *parent)
        : DspxParamInfoEntity(new DspxEnvParamInfoEntityExtra(), parent) {
    }
    DspxEnvParamInfoEntity::~DspxEnvParamInfoEntity() {
    }
    DspxParamCurveListEntity *DspxEnvParamInfoEntity::envelope() const {
        return static_cast<DspxEnvParamInfoEntityExtra *>(extra())->envelope;
    }
    //===========================================================================

    //===========================================================================
    // ParamSet
    class DspxParamSetEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxParamSetEntityExtra() : pitch(nullptr) {
        }

        void setup(AceTreeEntity *entity) override {
            Q_UNUSED(entity)
            addChildPointer("pitch", pitch);
        }

        DspxParamInfoEntity *pitch;
    };
    DspxParamSetEntity::DspxParamSetEntity(QObject *parent)
        : DspxParamSetEntity(new DspxParamSetEntityExtra(), parent) {
    }
    DspxParamSetEntity::~DspxParamSetEntity() {
    }
    DspxParamInfoEntity *DspxParamSetEntity::pitch() const {
        return static_cast<DspxParamSetEntityExtra *>(extra())->pitch;
    }
    DspxParamSetEntity::DspxParamSetEntity(DspxParamSetEntityExtra *extra, QObject *parent)
        : AceTreeEntityMapping(extra, parent) {
    }
    //===========================================================================

}