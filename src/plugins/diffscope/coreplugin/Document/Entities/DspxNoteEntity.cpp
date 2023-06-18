#include "DspxNoteEntity.h"
#include "AceTreeStandardEntity_p.h"
#include "DspxEntityUtils_p.h"

namespace Core {

    //===========================================================================
    // Phoneme
    class DspxPhonemeEntityPrivate : public AceTreeEntityMappingPrivate {
        Q_DECLARE_PUBLIC(DspxPhonemeEntity)
    public:
        DspxPhonemeEntityPrivate() {
            name = "phoneme";

            // Notifiers
            propertyNotifiers.insert("type", [](ACE_A) {
                ACE_Q(DspxPhonemeEntity);
                emit q->phonemeTypeChanged(variantToEnum<DspxPhonemeEntity::Type>(newValue));
            });
            propertyNotifiers.insert("token", [](ACE_A) {
                ACE_Q(DspxPhonemeEntity);
                emit q->tokenChanged(newValue.toString());
            });
            propertyNotifiers.insert("start", [](ACE_A) {
                ACE_Q(DspxPhonemeEntity);
                emit q->startChanged(newValue.toInt());
            });
        }
        ~DspxPhonemeEntityPrivate() = default;
    };

    DspxPhonemeEntity::DspxPhonemeEntity(QObject *parent) : DspxPhonemeEntity(*new DspxPhonemeEntityPrivate(), parent) {
    }
    DspxPhonemeEntity::~DspxPhonemeEntity() {
    }
    DspxPhonemeEntity::Type DspxPhonemeEntity::phonemeType() const {
        return variantToEnum<Type>(attribute("type"));
    }
    void DspxPhonemeEntity::setPhonemeType(DspxPhonemeEntity::Type type) {
        setAttribute("type", enumToString(type));
    }
    QString DspxPhonemeEntity::token() const {
        return attribute("token").toString();
    }
    void DspxPhonemeEntity::setToken(const QString &token) {
        setAttribute("token", token);
    }
    int DspxPhonemeEntity::start() const {
        return attribute("start").toInt();
    }
    void DspxPhonemeEntity::setStart(int start) {
        setAttribute("start", start);
    }
    DspxPhonemeEntity::DspxPhonemeEntity(DspxPhonemeEntityPrivate &d, QObject *parent)
        : AceTreeEntityMapping(d, parent) {
    }
    //===========================================================================

    //===========================================================================
    // EditedPhonemeList
    DspxPhonemeListEntity::DspxPhonemeListEntity(QObject *parent)
        : AceTreeEntityVector(parent), AceTreeEntityVectorHelper(this) {
        AceTreeStandardEntityPrivate::setName(this, "phonemes");
    }
    DspxPhonemeListEntity::~DspxPhonemeListEntity() {
    }
    //===========================================================================

    //===========================================================================
    // PhonemeInfo
    class DspxPhonemeInfoEntityPrivate : public AceTreeEntityMappingPrivate {
        Q_DECLARE_PUBLIC(DspxPhonemeInfoEntity)
    public:
        DspxPhonemeInfoEntityPrivate() {
            name = "phoneme";
            edited = nullptr;
            childPostAssignRefs.insert("edited", &edited);

            // Notifiers
            dynamicPropertyNotifiers.insert("original", [](ACE_A) {
                ACE_Q(DspxPhonemeInfoEntity);
                emit q->originalChanged(newValue.toJsonArray());
            });
        }
        ~DspxPhonemeInfoEntityPrivate() = default;

        DspxPhonemeListEntity *edited;
    };
    DspxPhonemeInfoEntity::DspxPhonemeInfoEntity(QObject *parent)
        : DspxPhonemeInfoEntity(*new DspxPhonemeInfoEntityPrivate(), parent) {
    }
    DspxPhonemeInfoEntity::~DspxPhonemeInfoEntity() {
    }
    QJsonArray DspxPhonemeInfoEntity::original() const {
        return attribute("original").toJsonArray();
    }
    void DspxPhonemeInfoEntity::setOriginal(const QJsonArray &original) {
        setAttribute("original", original);
    }
    DspxPhonemeListEntity *DspxPhonemeInfoEntity::edited() const {
        Q_D(const DspxPhonemeInfoEntity);
        return d->edited;
    }
    DspxPhonemeInfoEntity::DspxPhonemeInfoEntity(DspxPhonemeInfoEntityPrivate &d, QObject *parent)
        : AceTreeEntityMapping(d, parent) {
    }
    //===========================================================================

    //===========================================================================
    // VibratoInfo
    class DspxVibratoInfoEntityPrivate : public AceTreeEntityMappingPrivate {
        Q_DECLARE_PUBLIC(DspxVibratoInfoEntity)
    public:
        DspxVibratoInfoEntityPrivate() {
            name = "vibrato";
            points = nullptr;
            childPostAssignRefs.insert("points", &points);

            // Notifiers
            propertyNotifiers.insert("start", [](ACE_A) {
                ACE_Q(DspxVibratoInfoEntity);
                emit q->startChanged(newValue.toDouble());
            });
            propertyNotifiers.insert("end", [](ACE_A) {
                ACE_Q(DspxVibratoInfoEntity);
                emit q->endChanged(newValue.toDouble());
            });
            propertyNotifiers.insert("freq", [](ACE_A) {
                ACE_Q(DspxVibratoInfoEntity);
                emit q->frequencyChanged(newValue.toDouble());
            });
            propertyNotifiers.insert("phase", [](ACE_A) {
                ACE_Q(DspxVibratoInfoEntity);
                emit q->phaseChanged(newValue.toDouble());
            });
            propertyNotifiers.insert("amp", [](ACE_A) {
                ACE_Q(DspxVibratoInfoEntity);
                emit q->amplitudeChanged(newValue.toDouble());
            });
            propertyNotifiers.insert("offset", [](ACE_A) {
                ACE_Q(DspxVibratoInfoEntity);
                emit q->offsetChanged(newValue.toDouble());
            });
        }
        ~DspxVibratoInfoEntityPrivate() = default;
        DspxDoublePointListEntity *points;
    };
    DspxVibratoInfoEntity::DspxVibratoInfoEntity(QObject *parent)
        : AceTreeEntityMapping(*new DspxVibratoInfoEntityPrivate(), parent) {
    }
    DspxVibratoInfoEntity::~DspxVibratoInfoEntity() {
    }
    double DspxVibratoInfoEntity::start() const {
        return attribute("start").toDouble();
    }
    void DspxVibratoInfoEntity::setStart(double start) {
        setAttribute("start", start);
    }
    double DspxVibratoInfoEntity::end() const {
        return attribute("end").toDouble();
    }
    void DspxVibratoInfoEntity::setEnd(double end) {
        setAttribute("end", end);
    }
    double DspxVibratoInfoEntity::frequency() const {
        return attribute("freq").toDouble();
    }
    void DspxVibratoInfoEntity::setFrequency(double frequency) {
        setAttribute("freq", frequency);
    }
    double DspxVibratoInfoEntity::phase() const {
        return attribute("phase").toDouble();
    }
    void DspxVibratoInfoEntity::setPhase(double phase) {
        setAttribute("phase", phase);
    }
    double DspxVibratoInfoEntity::amplitude() const {
        return attribute("amp").toDouble();
    }
    void DspxVibratoInfoEntity::setAmplitude(double amplitude) {
        setAttribute("amp", amplitude);
    }
    double DspxVibratoInfoEntity::offset() const {
        return attribute("offset").toDouble();
    }
    void DspxVibratoInfoEntity::setOffset(double offset) {
        setAttribute("offset", offset);
    }
    DspxDoublePointListEntity *DspxVibratoInfoEntity::points() const {
        Q_D(const DspxVibratoInfoEntity);
        return d->points;
    }
    //===========================================================================

    //===========================================================================
    // Note
    class DspxNoteEntityPrivate : public AceTreeEntityMappingPrivate {
        Q_DECLARE_PUBLIC(DspxNoteEntity)
    public:
        DspxNoteEntityPrivate() {
            name = "note";
            phonemes = nullptr;
            vibrato = nullptr;
            childPostAssignRefs.insert("phonemes", &phonemes);
            childPostAssignRefs.insert("vibrato", &vibrato);

            // Notifiers
            propertyNotifiers.insert("pos", [](ACE_A) {
                ACE_Q(DspxNoteEntity);
                emit q->positionChanged(newValue.toInt());
            });
            propertyNotifiers.insert("length", [](ACE_A) {
                ACE_Q(DspxNoteEntity);
                emit q->lengthChanged(newValue.toInt());
            });
            propertyNotifiers.insert("keyNum", [](ACE_A) {
                ACE_Q(DspxNoteEntity);
                emit q->keyNumberChanged(newValue.toInt());
            });
            propertyNotifiers.insert("lyric", [](ACE_A) {
                ACE_Q(DspxNoteEntity);
                emit q->lyricChanged(newValue.toString());
            });
        }
        ~DspxNoteEntityPrivate() = default;
        DspxPhonemeInfoEntity *phonemes;
        DspxVibratoInfoEntity *vibrato;
    };
    DspxNoteEntity::DspxNoteEntity(QObject *parent) : AceTreeEntityMapping(*new DspxNoteEntityPrivate(), parent) {
    }
    DspxNoteEntity::~DspxNoteEntity() {
    }
    int DspxNoteEntity::position() const {
        return attribute("pos").toInt();
    }
    void DspxNoteEntity::setPosition(int position) {
        setAttribute("pos", position);
    }
    int DspxNoteEntity::length() const {
        return attribute("length").toInt();
    }
    void DspxNoteEntity::setLength(int length) {
        setAttribute("length", length);
    }
    int DspxNoteEntity::keyNumber() const {
        return attribute("keyNum").toInt();
    }
    void DspxNoteEntity::setKeyNumber(int keyNumber) {
        setAttribute("keyNum", keyNumber);
    }
    QString DspxNoteEntity::lyric() const {
        return attribute("lyric").toString();
    }
    void DspxNoteEntity::setLyric(const QString &lyric) {
        setAttribute("lyric", lyric);
    }
    DspxPhonemeInfoEntity *DspxNoteEntity::phonemes() const {
        Q_D(const DspxNoteEntity);
        return d->phonemes;
    }
    DspxVibratoInfoEntity *DspxNoteEntity::vibrato() const {
        Q_D(const DspxNoteEntity);
        return d->vibrato;
    }
    //===========================================================================

    //===========================================================================
    // NoteList
    DspxNoteListEntity::DspxNoteListEntity(QObject *parent)
        : AceTreeEntityRecordTable(parent), AceTreeEntityRecordTableHelper(this) {
        AceTreeStandardEntityPrivate::setName(this, "notes");
    }
    DspxNoteListEntity::~DspxNoteListEntity() {
    }
    void DspxNoteListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementPos<int>);
    }
    //===========================================================================

}