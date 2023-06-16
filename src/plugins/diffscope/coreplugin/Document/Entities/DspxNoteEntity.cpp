#include "DspxNoteEntity.h"
#include "AceTreeStandardEntity_p.h"
#include "DspxEntityUtils_p.h"

namespace Core {

    //===========================================================================
    // Phoneme
    class DspxPhonemeEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxPhonemeEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
            workspace = nullptr;
            childPostAssignRefs.insert("workspace", &workspace);
        }
        DspxWorkspaceEntity *workspace;
    };

    DspxPhonemeEntity::DspxPhonemeEntity(QObject *parent)
        : AceTreeStandardEntity(*new DspxPhonemeEntityPrivate(), parent) {
    }
    DspxPhonemeEntity::~DspxPhonemeEntity() {
    }
    QString DspxPhonemeEntity::name() const {
        return "phoneme";
    }
    DspxPhonemeEntity::Type DspxPhonemeEntity::phonemeType() const {
        return variantToEnum<Type>(property("type"));
    }
    void DspxPhonemeEntity::setPhonemeType(DspxPhonemeEntity::Type type) {
        setProperty("type", enumToString(type));
    }
    QString DspxPhonemeEntity::token() const {
        return property("token").toString();
    }
    void DspxPhonemeEntity::setToken(const QString &token) {
        setProperty("token", token);
    }
    int DspxPhonemeEntity::start() const {
        return property("start").toInt();
    }
    void DspxPhonemeEntity::setStart(int start) {
        setProperty("start", start);
    }
    DspxWorkspaceEntity *DspxPhonemeEntity::workspace() const {
        Q_D(const DspxPhonemeEntity);
        return d->workspace;
    }
    //===========================================================================

    //===========================================================================
    // EditedPhonemeList
    DspxPhonemeListEntity::DspxPhonemeListEntity(QObject *parent) : AceTreeEntityVector(parent) {
    }
    DspxPhonemeListEntity::~DspxPhonemeListEntity() {
    }
    QString DspxPhonemeListEntity::name() const {
        return "phonemes";
    }
    //===========================================================================

    //===========================================================================
    // PhonemeInfo
    class DspxPhonemeInfoEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxPhonemeInfoEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
            edited = nullptr;
            childPostAssignRefs.insert("edited", &edited);
        }
        DspxPhonemeListEntity *edited;
    };
    DspxPhonemeInfoEntity::DspxPhonemeInfoEntity(QObject *parent)
        : AceTreeStandardEntity(*new DspxPhonemeInfoEntityPrivate(), parent) {
    }
    DspxPhonemeInfoEntity::~DspxPhonemeInfoEntity() {
    }
    QString DspxPhonemeInfoEntity::name() const {
        return "phonemes";
    }
    QJsonArray DspxPhonemeInfoEntity::original() const {
        return property("original").toJsonArray();
    }
    void DspxPhonemeInfoEntity::setOriginal(const QJsonArray &original) {
        setProperty("original", original);
    }
    DspxPhonemeListEntity *DspxPhonemeInfoEntity::edited() const {
        Q_D(const DspxPhonemeInfoEntity);
        return d->edited;
    }
    //===========================================================================

    //===========================================================================
    // VibratoInfo
    class DspxVibratoInfoEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxVibratoInfoEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
            points = nullptr;
            childPostAssignRefs.insert("points", &points);
        }
        DspxDoublePointListEntity *points;
    };
    DspxVibratoInfoEntity::DspxVibratoInfoEntity(QObject *parent)
        : AceTreeStandardEntity(*new DspxVibratoInfoEntityPrivate(), parent) {
    }
    DspxVibratoInfoEntity::~DspxVibratoInfoEntity() {
    }
    QString DspxVibratoInfoEntity::name() const {
        return "vibrato";
    }
    double DspxVibratoInfoEntity::start() const {
        return property("start").toDouble();
    }
    void DspxVibratoInfoEntity::setStart(double start) {
        setProperty("start", start);
    }
    double DspxVibratoInfoEntity::end() const {
        return property("end").toDouble();
    }
    void DspxVibratoInfoEntity::setEnd(double end) {
        setProperty("end", end);
    }
    double DspxVibratoInfoEntity::frequency() const {
        return property("freq").toDouble();
    }
    void DspxVibratoInfoEntity::setFrequency(double frequency) {
        setProperty("freq", frequency);
    }
    double DspxVibratoInfoEntity::phase() const {
        return property("phase").toDouble();
    }
    void DspxVibratoInfoEntity::setPhase(double phase) {
        setProperty("phase", phase);
    }
    double DspxVibratoInfoEntity::amplitude() const {
        return property("amp").toDouble();
    }
    void DspxVibratoInfoEntity::setAmplitude(double amplitude) {
        setProperty("amp", amplitude);
    }
    double DspxVibratoInfoEntity::offset() const {
        return property("offset").toDouble();
    }
    void DspxVibratoInfoEntity::setOffset(double offset) {
        setProperty("offset", offset);
    }
    DspxDoublePointListEntity *DspxVibratoInfoEntity::points() const {
        Q_D(const DspxVibratoInfoEntity);
        return d->points;
    }
    //===========================================================================

    //===========================================================================
    // Note
    class DspxNoteEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxNoteEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
            phonemes = nullptr;
            vibrato = nullptr;
            workspace = nullptr;
            childPostAssignRefs.insert("phonemes", &phonemes);
            childPostAssignRefs.insert("vibrato", &vibrato);
            childPostAssignRefs.insert("workspace", &workspace);
        }
        DspxPhonemeInfoEntity *phonemes;
        DspxVibratoInfoEntity *vibrato;
        DspxWorkspaceEntity *workspace;
    };
    DspxNoteEntity::DspxNoteEntity(QObject *parent) : AceTreeStandardEntity(*new DspxNoteEntityPrivate(), parent) {
    }
    DspxNoteEntity::~DspxNoteEntity() {
    }
    QString DspxNoteEntity::name() const {
        return "note";
    }
    int DspxNoteEntity::position() const {
        return property("pos").toInt();
    }
    void DspxNoteEntity::setPosition(int position) {
        setProperty("pos", position);
    }
    int DspxNoteEntity::length() const {
        return property("length").toInt();
    }
    void DspxNoteEntity::setLength(int length) {
        setProperty("length", length);
    }
    int DspxNoteEntity::keyNumber() const {
        return property("keyNum").toInt();
    }
    void DspxNoteEntity::setKeyNumber(int keyNumber) {
        setProperty("keyNum", keyNumber);
    }
    QString DspxNoteEntity::lyric() const {
        return property("lyric").toString();
    }
    void DspxNoteEntity::setLyric(const QString &lyric) {
        setProperty("lyric", lyric);
    }
    DspxPhonemeInfoEntity *DspxNoteEntity::phonemes() const {
        Q_D(const DspxNoteEntity);
        return d->phonemes;
    }
    DspxVibratoInfoEntity *DspxNoteEntity::vibrato() const {
        Q_D(const DspxNoteEntity);
        return d->vibrato;
    }
    DspxWorkspaceEntity *DspxNoteEntity::workspace() const {
        Q_D(const DspxNoteEntity);
        return d->workspace;
    }
    //===========================================================================

    //===========================================================================
    // NoteList
    DspxNoteListEntity::DspxNoteListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
    }
    DspxNoteListEntity::~DspxNoteListEntity() {
    }
    QString DspxNoteListEntity::name() const {
        return "notes";
    }
    void DspxNoteListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), [](const AceTreeEntity *left, const AceTreeEntity *right) -> bool {
            return left->property("pos").toInt() < right->property("pos").toInt();
        });
    }
    //===========================================================================

}