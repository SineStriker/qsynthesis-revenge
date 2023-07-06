#include "DspxTimelineEntity.h"
#include "DspxEntityUtils_p.h"

#include <QJsonObject>

#include "AceTreeMacros.h"

namespace Core {

    //===========================================================================
    // Timeline
    class DspxTimelineEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxTimelineEntityExtra() : timeSignatures(nullptr), tempos(nullptr), labels(nullptr) {
        }
        void setup(AceTreeEntity *entity) override {
            Q_UNUSED(entity);

            addChildPointer("timeSignatures", timeSignatures);
            addChildPointer("tempos", tempos);
            addChildPointer("labels", labels);
        }
        DspxTimeSignatureListEntity *timeSignatures;
        DspxTempoListEntity *tempos;
        DspxTimelineLabelListEntity *labels;
    };
    DspxTimelineEntity::DspxTimelineEntity(QObject *parent)
        : AceTreeEntityMapping(new DspxTimelineEntityExtra(), parent) {
    }
    DspxTimelineEntity::~DspxTimelineEntity() {
    }
    DspxTimeSignatureListEntity *DspxTimelineEntity::timeSignatures() const {
        return static_cast<DspxTimelineEntityExtra *>(extra())->timeSignatures;
    }
    DspxTempoListEntity *DspxTimelineEntity::tempos() const {
        return static_cast<DspxTimelineEntityExtra *>(extra())->tempos;
    }
    DspxTimelineLabelListEntity *DspxTimelineEntity::labels() const {
        return static_cast<DspxTimelineEntityExtra *>(extra())->labels;
    }
    //===========================================================================

    //===========================================================================
    // TimeSignature
    class DspxTimeSignatureEntityExtra : public AceTreeEntityNotifyExtra {
    public:
        DspxTimeSignatureEntityExtra() : idx(0), num(0), den(0) {
        }

        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxTimeSignatureEntity *>(entity);

            addPropertyNotifier("p", [this, q](ACE_A) {
                Q_UNUSED(oldValue)
                idx = value.toInt();
                emit q->barIndexChanged(idx);
            });

            addPropertyNotifier("v", [this, q](ACE_A) {
                Q_UNUSED(oldValue)
                auto v = value.value<QsApi::MusicTimeSignature>();
                // Cache point value
                num = v.numerator();
                den = v.denominator();
                emit q->valueChanged(num, den);
            });
        }

        int idx, num, den; // avoid conversion when read data
    };
    DspxTimeSignatureEntity::DspxTimeSignatureEntity(QObject *parent)
        : AceTreeEntity(new DspxTimeSignatureEntityExtra(), parent) {
    }
    DspxTimeSignatureEntity::~DspxTimeSignatureEntity() {
    }
    bool DspxTimeSignatureEntity::read(const QJsonValue &value) {
        if (!value.isObject())
            return false;
        auto obj = value.toObject();
        setBarIndex(obj.value("index").toInt());

        auto v = QsApi::MusicTimeSignature(obj.value("numerator").toInt(), obj.value("denominator").toInt());
        setValue(v.isValid() ? v : QsApi::MusicTimeSignature());
        return true;
    }
    QJsonValue DspxTimeSignatureEntity::write() const {
        auto extra = static_cast<DspxTimeSignatureEntityExtra *>(this->extra());
        return QJsonObject{
            {"index",       extra->idx},
            {"numerator",   extra->num},
            {"denominator", extra->den},
        };
    }
    int DspxTimeSignatureEntity::barIndex() const {
        return static_cast<DspxTimeSignatureEntityExtra *>(extra())->idx;
    }
    void DspxTimeSignatureEntity::setBarIndex(int barIndex) {
        treeItem()->setAttribute("p", barIndex);
    }
    int DspxTimeSignatureEntity::numerator() const {
        return static_cast<DspxTimeSignatureEntityExtra *>(extra())->num;
    }
    int DspxTimeSignatureEntity::denominator() const {
        return static_cast<DspxTimeSignatureEntityExtra *>(extra())->den;
    }
    QsApi::MusicTimeSignature DspxTimeSignatureEntity::value() const {
        auto extra = static_cast<DspxTimeSignatureEntityExtra *>(this->extra());
        return {extra->num, extra->den};
    }
    void DspxTimeSignatureEntity::setValue(const QsApi::MusicTimeSignature &value) {
        if (!value.isValid())
            return;
        treeItem()->setAttribute("v", QVariant::fromValue(value));
    }
    void DspxTimeSignatureEntity::doInitialize() {
        setBarIndex(0);
        setValue({});
    }
    void DspxTimeSignatureEntity::doSetup() {
        auto v = treeItem()->attribute("v").value<QsApi::MusicTimeSignature>();
        auto extra = static_cast<DspxTimeSignatureEntityExtra *>(this->extra());

        // Get cache
        extra->num = v.numerator();
        extra->den = v.denominator();
    }
    //===========================================================================

    //===========================================================================
    // TimeSignature List
    DspxTimeSignatureListEntity::DspxTimeSignatureListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
    }
    DspxTimeSignatureListEntity::~DspxTimeSignatureListEntity() {
    }
    void DspxTimeSignatureListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), [](const AceTreeEntity *left, const AceTreeEntity *right) {
            return static_cast<const DspxTimeSignatureEntity *>(left)->barIndex() <
                   static_cast<const DspxTimeSignatureEntity *>(right)->barIndex();
        });
    }
    //===========================================================================

    //===========================================================================
    // Tempo
    class DspxTempoEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxTempoEntityExtra() {
        }
        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxTempoEntity *>(entity);

            addPropertyNotifier("pos", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->positionChanged(value.toInt());
            });
            addPropertyNotifier("value", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->valueChanged(value.toDouble());
            });
        }
    };
    DspxTempoEntity::DspxTempoEntity(QObject *parent) : AceTreeEntityMapping(new DspxTempoEntityExtra(), parent) {
    }
    DspxTempoEntity::~DspxTempoEntity() {
    }
    int DspxTempoEntity::position() const {
        return attribute("pos").toInt();
    }
    void DspxTempoEntity::setPosition(int position) {
        setAttribute("pos", position);
    }
    double DspxTempoEntity::value() const {
        return attribute("value").toDouble();
    }
    void DspxTempoEntity::setValue(double value) {
        setAttribute("value", value);
    }
    //===========================================================================

    //===========================================================================
    // Tempo List
    DspxTempoListEntity::DspxTempoListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
    }
    DspxTempoListEntity::~DspxTempoListEntity() {
    }
    void DspxTempoListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementPos<DspxTempoEntity>);
    }
    //===========================================================================

    //===========================================================================
    // TimelineLabel
    class DspxTimelineLabelEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxTimelineLabelEntityExtra() {
        }
        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxTimelineLabelEntity *>(entity);

            addPropertyNotifier("pos", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->positionChanged(value.toInt());
            });
            addPropertyNotifier("text", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->textChanged(value.toString());
            });
        }
    };
    DspxTimelineLabelEntity::DspxTimelineLabelEntity(QObject *parent)
        : AceTreeEntityMapping(new DspxTimelineLabelEntityExtra(), parent) {
    }
    DspxTimelineLabelEntity::~DspxTimelineLabelEntity() {
    }
    int DspxTimelineLabelEntity::position() const {
        return attribute("pos").toInt();
    }
    void DspxTimelineLabelEntity::setPosition(int position) {
        setAttribute("pos", position);
    }
    QString DspxTimelineLabelEntity::text() const {
        return attribute("text").toString();
    }
    void DspxTimelineLabelEntity::setText(const QString &text) {
        setAttribute("text", text);
    }
    //===========================================================================

    //===========================================================================
    DspxTimelineLabelListEntity::DspxTimelineLabelListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
    }
    DspxTimelineLabelListEntity::~DspxTimelineLabelListEntity() {
    }

    void DspxTimelineLabelListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementPos<DspxTimelineLabelEntity>);
    }
    //===========================================================================

} // namespace Core
