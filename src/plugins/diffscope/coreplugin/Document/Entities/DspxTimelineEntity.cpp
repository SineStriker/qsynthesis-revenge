#include "DspxTimelineEntity.h"
#include "AceTreeStandardEntity_p.h"

namespace Core {

    static bool compareTimelineElement(const AceTreeEntity *left, const AceTreeEntity *right) {
        return left->property("pos").toInt() < right->property("pos").toInt();
    }

    //===========================================================================
    // Timeline
    class DspxTimelineEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxTimelineEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
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
    QString DspxTimelineEntity::name() const {
        return "timeline";
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
    }
    DspxTimeSignatureEntity::~DspxTimeSignatureEntity() {
    }
    QString DspxTimeSignatureEntity::name() const {
        return "timeSignature";
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
    }
    DspxTimeSignatureListEntity::~DspxTimeSignatureListEntity() {
    }
    QString DspxTimeSignatureListEntity::name() const {
        return "timeSignatures";
    }
    void DspxTimeSignatureListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareTimelineElement);
    }
    //===========================================================================

    //===========================================================================
    // Tempo
    DspxTempoEntity::DspxTempoEntity(QObject *parent) : AceTreeEntityMapping(parent) {
    }
    DspxTempoEntity::~DspxTempoEntity() {
    }
    QString DspxTempoEntity::name() const {
        return "tempo";
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
    DspxTempoListEntity::DspxTempoListEntity(QObject *parent) : AceTreeEntityVector(parent) {
    }
    DspxTempoListEntity::~DspxTempoListEntity() {
    }
    QString DspxTempoListEntity::name() const {
        return "tempos";
    }
    void DspxTempoListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareTimelineElement);
    }
    //===========================================================================

    //===========================================================================
    // TimelineLabel
    DspxTimelineLabelEntity::DspxTimelineLabelEntity(QObject *parent) : AceTreeEntityMapping(parent) {
    }
    DspxTimelineLabelEntity::~DspxTimelineLabelEntity() {
    }
    QString DspxTimelineLabelEntity::name() const {
        return "label";
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
    DspxTimelineLabelListEntity::DspxTimelineLabelListEntity(QObject *parent) : AceTreeEntityVector(parent) {
    }
    DspxTimelineLabelListEntity::~DspxTimelineLabelListEntity() {
    }
    QString DspxTimelineLabelListEntity::name() const {
        return "labels";
    }
    void DspxTimelineLabelListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareTimelineElement);
    }
    //===========================================================================

}