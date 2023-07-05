#include "DspxTimelineEntity.h"
#include "DspxEntityUtils_p.h"

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
    class DspxTimeSignatureEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxTimeSignatureEntityExtra() {
        }
        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxTimeSignatureEntity *>(entity);

            addPropertyNotifier("pos", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->positionChanged(value.toInt());
            });
            addPropertyNotifier("num", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->numeratorChanged(value.toInt());
            });
            addPropertyNotifier("den", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->denominatorChanged(value.toInt());
            });
        }
    };
    DspxTimeSignatureEntity::DspxTimeSignatureEntity(QObject *parent)
        : AceTreeEntityMapping(new DspxTimeSignatureEntityExtra(), parent) {
    }
    DspxTimeSignatureEntity::~DspxTimeSignatureEntity() {
    }
    int DspxTimeSignatureEntity::position() const {
        return attribute("pos").toInt();
    }
    void DspxTimeSignatureEntity::setPosition(int position) {
        setAttribute("pos", position);
    }
    int DspxTimeSignatureEntity::numerator() const {
        return attribute("num").toInt();
    }
    void DspxTimeSignatureEntity::setNumerator(int numerator) {
        setAttribute("num", numerator);
    }
    int DspxTimeSignatureEntity::denominator() const {
        return attribute("den").toInt();
    }
    void DspxTimeSignatureEntity::setDenominator(int denominator) {
        setAttribute("den", denominator);
    }
    //===========================================================================

    //===========================================================================
    // TimeSignature List
    DspxTimeSignatureListEntity::DspxTimeSignatureListEntity(QObject *parent)
        : AceTreeEntityRecordTable(parent) {
    }
    DspxTimeSignatureListEntity::~DspxTimeSignatureListEntity() {
    }
    void DspxTimeSignatureListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementPos<DspxTimeSignatureEntity>);
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
    DspxTempoEntity::DspxTempoEntity(QObject *parent)
        : AceTreeEntityMapping(new DspxTempoEntityExtra(), parent) {
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
    // TimelineLabel List
    class DspxTimelineLabelListEntityExtra : public AceTreeEntityExtra {
    public:
    };

    DspxTimelineLabelListEntity::DspxTimelineLabelListEntity(QObject *parent)
        : AceTreeEntityRecordTable(parent) {
    }
    DspxTimelineLabelListEntity::~DspxTimelineLabelListEntity() {
    }

    const QsApi::MusicTimeline *DspxTimelineLabelListEntity::timeline() const {
        return nullptr;
    }

    void DspxTimelineLabelListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementPos<DspxTimelineLabelEntity>);
    }

    void DspxTimelineLabelListEntity::doInitialize() {
    }

    void DspxTimelineLabelListEntity::doSetup() {
    }
    //===========================================================================

} // namespace Core
