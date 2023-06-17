#include "DspxTimelineEntity.h"
#include "AceTreeStandardEntity_p.h"
#include "DspxEntityUtils_p.h"

namespace Core {

    //===========================================================================
    // Timeline
    class DspxTimelineEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxTimelineEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
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
        : AceTreeStandardEntity(*new DspxTimelineEntityPrivate(), parent) {
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
        AceTreeStandardEntityPrivate::setName(this, "timeSignature");
    }
    DspxTimeSignatureEntity::~DspxTimeSignatureEntity() {
    }
    int DspxTimeSignatureEntity::position() const {
        return property("pos").toInt();
    }
    void DspxTimeSignatureEntity::setPosition(int position) {
        setProperty("pos", position);
    }
    int DspxTimeSignatureEntity::numerator() const {
        return property("numerator").toInt();
    }
    void DspxTimeSignatureEntity::setNumerator(int numerator) {
        setProperty("numerator", numerator);
    }
    int DspxTimeSignatureEntity::denominator() const {
        return property("denominator").toInt();
    }
    void DspxTimeSignatureEntity::setDenominator(int denominator) {
        setProperty("denominator", denominator);
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
        AceTreeStandardEntityPrivate::setName(this, "tempo");
    }
    DspxTempoEntity::~DspxTempoEntity() {
    }
    int DspxTempoEntity::position() const {
        return property("pos").toInt();
    }
    void DspxTempoEntity::setPosition(int position) {
        setProperty("pos", position);
    }
    double DspxTempoEntity::value() const {
        return property("value").toDouble();
    }
    void DspxTempoEntity::setValue(double value) {
        setProperty("value", value);
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
        AceTreeStandardEntityPrivate::setName(this, "label");
    }
    DspxTimelineLabelEntity::~DspxTimelineLabelEntity() {
    }
    int DspxTimelineLabelEntity::position() const {
        return property("pos").toInt();
    }
    void DspxTimelineLabelEntity::setPosition(int position) {
        setProperty("pos", position);
    }
    QString DspxTimelineLabelEntity::text() const {
        return property("text").toString();
    }
    void DspxTimelineLabelEntity::setText(const QString &text) {
        setProperty("text", text);
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