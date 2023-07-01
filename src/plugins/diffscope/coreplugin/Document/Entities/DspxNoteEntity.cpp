#include "DspxNoteEntity.h"
#include "DspxEntityUtils_p.h"

#include <QJsonObject>

#include "AceTreeMacros.h"

namespace Core {

    //===========================================================================
    // Phoneme
    class DspxPhonemeEntityExtra : public AceTreeEntityMappingExtra {
    public:
        void setup(AceTreeEntity *entity) override {

            auto q = static_cast<DspxPhonemeEntity *>(entity);
            addPropertyNotifier("type", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->phonemeTypeChanged(variantToEnum<DspxPhonemeEntity::Type>(value));
            });
            addPropertyNotifier("token", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->tokenChanged(value.toString());
            });
            addPropertyNotifier("start", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->startChanged(value.toInt());
            });
        }
    };

    DspxPhonemeEntity::DspxPhonemeEntity(QObject *parent) : AceTreeEntityMapping(new DspxPhonemeEntityExtra(), parent) {
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
    //===========================================================================

    //===========================================================================
    // EditedPhonemeList
    DspxPhonemeListEntity::DspxPhonemeListEntity(QObject *parent) : AceTreeEntityVector(parent) {
    }
    DspxPhonemeListEntity::~DspxPhonemeListEntity() {
    }
    //===========================================================================

    //===========================================================================
    // PhonemeInfo
    class DspxPhonemeInfoEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxPhonemeInfoEntityExtra() : edited(nullptr) {
        }
        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxPhonemeInfoEntity *>(entity);

            addChildPointer("edited", edited);

            addDynamicDataNotifier("original", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->originalChanged(value.toJsonArray());
            });
        }
        DspxPhonemeListEntity *edited;
    };

    DspxPhonemeInfoEntity::DspxPhonemeInfoEntity(QObject *parent)
        : AceTreeEntityMapping(new DspxPhonemeInfoEntityExtra(), parent) {
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
        return static_cast<DspxPhonemeInfoEntityExtra *>(extra())->edited;
    }
    //===========================================================================

    //===========================================================================
    // VibratoInfo
    class DspxVibratoInfoEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxVibratoInfoEntityExtra() : points(nullptr) {
        }
        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxVibratoInfoEntity *>(entity);

            addChildPointer("points", points);

            addPropertyNotifier("start", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->startChanged(value.toDouble());
            });
            addPropertyNotifier("end", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->endChanged(value.toDouble());
            });
            addPropertyNotifier("freq", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->frequencyChanged(value.toDouble());
            });
            addPropertyNotifier("phase", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->phaseChanged(value.toDouble());
            });
            addPropertyNotifier("amp", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->amplitudeChanged(value.toDouble());
            });
            addPropertyNotifier("offset", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->offsetChanged(value.toDouble());
            });
        }
        DspxDoublePointListEntity *points;
    };
    DspxVibratoInfoEntity::DspxVibratoInfoEntity(QObject *parent)
        : AceTreeEntityMapping(new DspxVibratoInfoEntityExtra(), parent) {
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
        return static_cast<DspxVibratoInfoEntityExtra *>(extra())->points;
    }
    //===========================================================================

    //===========================================================================
    // Note
    class DspxNoteEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxNoteEntityExtra() : phonemes(nullptr), vibrato(nullptr) {
        }
        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxNoteEntity *>(entity);

            addChildPointer("phonemes", phonemes);
            addChildPointer("vibrato", vibrato);

            addPropertyNotifier("pos", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->positionChanged(value.toInt());
            });
            addPropertyNotifier("length", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->lengthChanged(value.toInt());
            });
            addPropertyNotifier("keyNum", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->keyNumberChanged(value.toInt());
            });
            addPropertyNotifier("lyric", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->lyricChanged(value.toString());
            });
        }
        DspxPhonemeInfoEntity *phonemes;
        DspxVibratoInfoEntity *vibrato;
    };
    DspxNoteEntity::DspxNoteEntity(QObject *parent) : AceTreeEntityMapping(new DspxNoteEntityExtra(), parent) {
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
        return static_cast<DspxNoteEntityExtra *>(extra())->phonemes;
    }
    DspxVibratoInfoEntity *DspxNoteEntity::vibrato() const {
        return static_cast<DspxNoteEntityExtra *>(extra())->vibrato;
    }
    //===========================================================================

    //===========================================================================
    // NoteList
    DspxNoteListEntity::DspxNoteListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
    }
    DspxNoteListEntity::~DspxNoteListEntity() {
    }
    void DspxNoteListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementPos<DspxNoteEntity>);
    }
    //===========================================================================

}