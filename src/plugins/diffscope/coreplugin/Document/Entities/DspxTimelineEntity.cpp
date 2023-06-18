#include "DspxTimelineEntity.h"
#include "AceTreeStandardEntity_p.h"
#include "DspxEntityUtils_p.h"

namespace Core {

    //===========================================================================
    // Timeline
    class DspxTimelineEntityPrivate : public AceTreeEntityMappingPrivate {
        Q_DECLARE_PUBLIC(DspxTimelineEntity)
    public:
        DspxTimelineEntityPrivate() {
            name = "timeline";
            timeSignatures = nullptr;
            tempos = nullptr;
            labels = nullptr;
            childPostAssignRefs.insert("timeSignatures", &timeSignatures);
            childPostAssignRefs.insert("tempos", &tempos);
            childPostAssignRefs.insert("labels", &labels);
        }
        ~DspxTimelineEntityPrivate() = default;

        DspxTimeSignatureListEntity *timeSignatures;
        DspxTempoListEntity *tempos;
        DspxTimelineLabelListEntity *labels;
    };
    DspxTimelineEntity::DspxTimelineEntity(QObject *parent)
        : AceTreeEntityMapping(*new DspxTimelineEntityPrivate(), parent) {
    }
    DspxTimelineEntity::~DspxTimelineEntity() {
    }
    DspxTimeSignatureListEntity *DspxTimelineEntity::timeSignatures() const {
        Q_D(const DspxTimelineEntity);
        return d->timeSignatures;
    }
    DspxTempoListEntity *DspxTimelineEntity::tempos() const {
        Q_D(const DspxTimelineEntity);
        return d->tempos;
    }
    DspxTimelineLabelListEntity *DspxTimelineEntity::labels() const {
        Q_D(const DspxTimelineEntity);
        return d->labels;
    }
    //===========================================================================

    //===========================================================================
    // TimeSignature
    DspxTimeSignatureEntity::DspxTimeSignatureEntity(QObject *parent) : AceTreeEntityMapping(parent) {
        auto d = AceTreeEntityMappingPrivate::get(this);
        d->name = "timeSignature";

        // Notifiers
        d->propertyNotifiers.insert("pos", [](ACE_A) {
            ACE_Q(DspxTimeSignatureEntity);
            emit q->positionChanged(newValue.toInt());
        });
        d->propertyNotifiers.insert("num", [](ACE_A) {
            ACE_Q(DspxTimeSignatureEntity);
            emit q->numeratorChanged(newValue.toInt());
        });
        d->propertyNotifiers.insert("den", [](ACE_A) {
            ACE_Q(DspxTimeSignatureEntity);
            emit q->denominatorChanged(newValue.toInt());
        });
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
    DspxTimeSignatureListEntity::DspxTimeSignatureListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
        AceTreeStandardEntityPrivate::setName(this, "timeSignatures");
    }
    DspxTimeSignatureListEntity::~DspxTimeSignatureListEntity() {
    }
    void DspxTimeSignatureListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementPos<int>);
    }
    //===========================================================================

    //===========================================================================
    // Tempo
    DspxTempoEntity::DspxTempoEntity(QObject *parent) : AceTreeEntityMapping(parent) {
        auto d = AceTreeEntityMappingPrivate::get(this);
        d->name = "tempo";

        // Notifiers
        d->propertyNotifiers.insert("pos", [](ACE_A) {
            ACE_Q(DspxTempoEntity);
            emit q->positionChanged(newValue.toInt());
        });
        d->propertyNotifiers.insert("value", [](ACE_A) {
            ACE_Q(DspxTempoEntity);
            emit q->valueChanged(newValue.toDouble());
        });
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
        AceTreeStandardEntityPrivate::setName(this, "tempos");
    }
    DspxTempoListEntity::~DspxTempoListEntity() {
    }
    void DspxTempoListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementPos<int>);
    }
    //===========================================================================

    //===========================================================================
    // TimelineLabel
    DspxTimelineLabelEntity::DspxTimelineLabelEntity(QObject *parent) : AceTreeEntityMapping(parent) {
        auto d = AceTreeEntityMappingPrivate::get(this);
        d->name = "label";

        // Notifiers
        d->propertyNotifiers.insert("pos", [](ACE_A) {
            ACE_Q(DspxTimelineLabelEntity);
            emit q->positionChanged(newValue.toInt());
        });
        d->propertyNotifiers.insert("text", [](ACE_A) {
            ACE_Q(DspxTimelineLabelEntity);
            emit q->textChanged(newValue.toString());
        });
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
    DspxTimelineLabelListEntity::DspxTimelineLabelListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
        AceTreeStandardEntityPrivate::setName(this, "labels");
    }
    DspxTimelineLabelListEntity::~DspxTimelineLabelListEntity() {
    }
    void DspxTimelineLabelListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementPos<int>);
    }
    //===========================================================================

}