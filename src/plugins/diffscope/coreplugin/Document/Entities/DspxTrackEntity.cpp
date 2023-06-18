#include "DspxTrackEntity.h"
#include "AceTreeStandardEntity_p.h"
#include "DspxEntityUtils_p.h"

namespace Core {

    //===========================================================================
    // ClipTime
    DspxClipTimeEntity::DspxClipTimeEntity(QObject *parent) : AceTreeEntityMapping(parent) {
        auto d = AceTreeEntityMappingPrivate::get(this);
        d->name = "time";

        // Notifiers
        d->propertyNotifiers.insert("start", [](ACE_A) {
            ACE_Q(DspxClipTimeEntity);
            emit q->startChanged(newValue.toInt());
        });
        d->propertyNotifiers.insert("length", [](ACE_A) {
            ACE_Q(DspxClipTimeEntity);
            emit q->lengthChanged(newValue.toInt());
        });
        d->propertyNotifiers.insert("clipStart", [](ACE_A) {
            ACE_Q(DspxClipTimeEntity);
            emit q->clipStartChanged(newValue.toInt());
        });
        d->propertyNotifiers.insert("clipLen", [](ACE_A) {
            ACE_Q(DspxClipTimeEntity);
            emit q->clipLengthChanged(newValue.toInt());
        });
    }
    DspxClipTimeEntity::~DspxClipTimeEntity() {
    }
    int DspxClipTimeEntity::start() const {
        return attribute("start").toInt();
    }
    void DspxClipTimeEntity::setStart(int start) {
        setAttribute("start", start);
    }
    int DspxClipTimeEntity::length() const {
        return attribute("length").toInt();
    }
    void DspxClipTimeEntity::setLength(int length) {
        setAttribute("length", length);
    }
    int DspxClipTimeEntity::clipStart() const {
        return attribute("clipStart").toInt();
    }
    void DspxClipTimeEntity::setClipStart(int clipStart) {
        setAttribute("clipStart", clipStart);
    }
    int DspxClipTimeEntity::clipLength() const {
        return attribute("clipLen").toInt();
    }
    void DspxClipTimeEntity::setClipLength(int clipLength) {
        setAttribute("clipLen", clipLength);
    }
    //===========================================================================

    //===========================================================================
    // Clip
    class DspxClipEntityPrivate : public AceTreeEntityMappingPrivate {
        Q_DECLARE_PUBLIC(DspxClipEntity)
    public:
        DspxClipEntityPrivate(DspxClipEntity::Type type) : type(type) {
            name = "clip";
            time = nullptr;
            control = nullptr;
            childPostAssignRefs.insert("time", &time);
            childPostAssignRefs.insert("control", &control);

            // Notifiers
            propertyNotifiers.insert("name", [](ACE_A) {
                ACE_Q(DspxClipEntity);
                emit q->clipNameChanged(newValue.toString());
            });
        }
        ~DspxClipEntityPrivate() = default;

        DspxClipEntity::Type type;

        DspxClipTimeEntity *time;
        DspxBusControlEntity *control;
    };
    DspxClipEntity::DspxClipEntity(DspxClipEntity::Type type, QObject *parent)
        : DspxClipEntity(*new DspxClipEntityPrivate(type), parent) {
    }
    DspxClipEntity::~DspxClipEntity() {
    }
    DspxClipEntity::Type DspxClipEntity::clipType() const {
        Q_D(const DspxClipEntity);
        return d->type;
    }
    QString DspxClipEntity::clipName() const {
        return attribute("name").toString();
    }
    void DspxClipEntity::setClipName(const QString &clipName) {
        setAttribute("name", clipName);
    }
    DspxClipTimeEntity *DspxClipEntity::time() const {
        Q_D(const DspxClipEntity);
        return d->time;
    }
    DspxBusControlEntity *DspxClipEntity::control() const {
        Q_D(const DspxClipEntity);
        return d->control;
    }
    DspxClipEntity::DspxClipEntity(DspxClipEntityPrivate &d, QObject *parent) : AceTreeEntityMapping(d, parent) {
    }
    //===========================================================================

    //===========================================================================
    // AudioClip
    class DspxAudioClipEntityPrivate : public DspxClipEntityPrivate {
        Q_DECLARE_PUBLIC(DspxAudioClipEntity)
    public:
        DspxAudioClipEntityPrivate() : DspxClipEntityPrivate(DspxClipEntity::Audio) {
            name = "audio";

            // Notifiers
            propertyNotifiers.insert("path", [](ACE_A) {
                ACE_Q(DspxAudioClipEntity);
                emit q->pathChanged(newValue.toString());
            });
        }
        ~DspxAudioClipEntityPrivate() = default;
    };
    DspxAudioClipEntity::DspxAudioClipEntity(QObject *parent)
        : DspxClipEntity(*new DspxAudioClipEntityPrivate(), parent) {
    }
    DspxAudioClipEntity::~DspxAudioClipEntity() {
    }
    QString DspxAudioClipEntity::path() const {
        return attribute("path").toString();
    }
    void DspxAudioClipEntity::setPath(const QString &path) {
        setAttribute("path", path);
    }
    //===========================================================================

    //===========================================================================
    // SingingClip
    class DspxSingingClipEntityPrivate : public DspxClipEntityPrivate {
    public:
        DspxSingingClipEntityPrivate() : DspxClipEntityPrivate(DspxClipEntity::Singing) {
            name = "singing";
            notes = nullptr;
            params = nullptr;
            childPostAssignRefs.insert("notes", &notes);
            childPostAssignRefs.insert("params", &params);
        }
        ~DspxSingingClipEntityPrivate() = default;

        DspxNoteListEntity *notes;
        DspxParamSetEntity *params;
    };
    DspxSingingClipEntity::DspxSingingClipEntity(QObject *parent)
        : DspxClipEntity(*new DspxSingingClipEntityPrivate(), parent) {
    }
    DspxSingingClipEntity::~DspxSingingClipEntity() {
    }
    DspxParamSetEntity *DspxSingingClipEntity::params() const {
        Q_D(const DspxSingingClipEntity);
        return d->params;
    }
    DspxNoteListEntity *DspxSingingClipEntity::notes() const {
        Q_D(const DspxSingingClipEntity);
        return d->notes;
    }
    //===========================================================================

    //===========================================================================
    // Clip List
    DspxClipListEntity::DspxClipListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
        AceTreeStandardEntityPrivate::setName(this, "clips");
    }
    DspxClipListEntity::~DspxClipListEntity() {
    }
    void DspxClipListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementStart<int>);
    }
    //===========================================================================

    //===========================================================================
    // Track
    class DspxTrackEntityPrivate : public AceTreeEntityMappingPrivate {
        Q_DECLARE_PUBLIC(DspxTrackEntity)
    public:
        DspxTrackEntityPrivate() {
            name = "track";
            control = nullptr;
            clips = nullptr;
            childPostAssignRefs.insert("control", &control);
            childPostAssignRefs.insert("clips", &clips);

            // Notifiers
            propertyNotifiers.insert("name", [](ACE_A) {
                ACE_Q(DspxTrackEntity);
                emit q->trackNameChanged(newValue.toString());
            });
        }
        ~DspxTrackEntityPrivate() = default;

        DspxTrackControlEntity *control;
        DspxClipListEntity *clips;
    };
    DspxTrackEntity::DspxTrackEntity(QObject *parent) : AceTreeEntityMapping(*new DspxTrackEntityPrivate(), parent) {
    }
    DspxTrackEntity::~DspxTrackEntity() {
    }
    QString DspxTrackEntity::trackName() const {
        return attribute("name").toString();
    }
    void DspxTrackEntity::setTrackName(const QString &trackName) {
        setAttribute("name", trackName);
    }
    QJsonObject DspxTrackEntity::colorConfiguration() const {
        return attribute("color").toJsonObject();
    }
    void DspxTrackEntity::setColorConfiguration(const QJsonObject &colorConfiguration) {
        setAttribute("color", colorConfiguration);
    }
    DspxTrackControlEntity *DspxTrackEntity::control() const {
        Q_D(const DspxTrackEntity);
        return d->control;
    }
    DspxClipListEntity *DspxTrackEntity::clips() const {
        Q_D(const DspxTrackEntity);
        return d->clips;
    }
    //===========================================================================

    //===========================================================================
    // Track List
    DspxTrackListEntity::DspxTrackListEntity(QObject *parent) : AceTreeEntityVector(parent) {
        AceTreeStandardEntityPrivate::setName(this, "tracks");
    }
    DspxTrackListEntity::~DspxTrackListEntity() {
    }
    //===========================================================================

}