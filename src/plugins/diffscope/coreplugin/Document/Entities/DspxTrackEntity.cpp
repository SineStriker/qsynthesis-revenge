#include "DspxTrackEntity.h"
#include "AceTreeStandardEntity_p.h"
#include "DspxEntityUtils_p.h"

namespace Core {

    //===========================================================================
    // ClipTime
    DspxClipTimeEntity::DspxClipTimeEntity(QObject *parent) : AceTreeEntityMapping(parent) {
    }
    DspxClipTimeEntity::~DspxClipTimeEntity() {
    }
    QString DspxClipTimeEntity::name() const {
        return "time";
    }
    int DspxClipTimeEntity::start() const {
        return property("start").toInt();
    }
    void DspxClipTimeEntity::setStart(int start) {
        setProperty("start", start);
    }
    int DspxClipTimeEntity::length() const {
        return property("length").toInt();
    }
    void DspxClipTimeEntity::setLength(int length) {
        setProperty("length", length);
    }
    int DspxClipTimeEntity::clipStart() const {
        return property("clipStart").toInt();
    }
    void DspxClipTimeEntity::setClipStart(int clipStart) {
        setProperty("clipStart", clipStart);
    }
    int DspxClipTimeEntity::clipLength() const {
        return property("clipLen").toInt();
    }
    void DspxClipTimeEntity::setClipLength(int clipLength) {
        setProperty("clipLen", clipLength);
    }
    //===========================================================================

    //===========================================================================
    // Clip
    class DspxClipEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxClipEntityPrivate(DspxClipEntity::Type type)
            : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping), type(type) {
            name = "clip";
            time = nullptr;
            control = nullptr;
            childPostAssignRefs.insert("time", &time);
            childPostAssignRefs.insert("control", &control);
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
        return property("name").toString();
    }
    void DspxClipEntity::setClipName(const QString &clipName) {
        setProperty("name", clipName);
    }
    DspxClipTimeEntity *DspxClipEntity::time() const {
        Q_D(const DspxClipEntity);
        return d->time;
    }
    DspxBusControlEntity *DspxClipEntity::control() const {
        Q_D(const DspxClipEntity);
        return d->control;
    }
    DspxClipEntity::DspxClipEntity(DspxClipEntityPrivate &d, QObject *parent) : AceTreeStandardEntity(d, parent) {
    }
    //===========================================================================

    //===========================================================================
    // AudioClip
    class DspxAudioClipEntityPrivate : public DspxClipEntityPrivate {
    public:
        DspxAudioClipEntityPrivate() : DspxClipEntityPrivate(DspxClipEntity::Audio) {
            name = "audio";
        }
        ~DspxAudioClipEntityPrivate() = default;
    };
    DspxAudioClipEntity::DspxAudioClipEntity(QObject *parent)
        : DspxClipEntity(*new DspxAudioClipEntityPrivate(), parent) {
    }
    DspxAudioClipEntity::~DspxAudioClipEntity() {
    }
    QString DspxAudioClipEntity::path() const {
        return property("path").toString();
    }
    void DspxAudioClipEntity::setPath(const QString &path) {
        setProperty("path", path);
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
    class DspxTrackEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxTrackEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
            name = "track";
            control = nullptr;
            clips = nullptr;
            childPostAssignRefs.insert("control", &control);
            childPostAssignRefs.insert("clips", &clips);
        }
        ~DspxTrackEntityPrivate() = default;

        DspxTrackControlEntity *control;
        DspxClipListEntity *clips;
    };
    DspxTrackEntity::DspxTrackEntity(QObject *parent) : AceTreeStandardEntity(*new DspxTrackEntityPrivate(), parent) {
    }
    DspxTrackEntity::~DspxTrackEntity() {
    }
    QString DspxTrackEntity::trackName() const {
        return property("name").toString();
    }
    void DspxTrackEntity::setTrackName(const QString &trackName) {
        setProperty("name", trackName);
    }
    QJsonObject DspxTrackEntity::colorConfiguration() const {
        return property("color").toJsonObject();
    }
    void DspxTrackEntity::setColorConfiguration(const QJsonObject &colorConfiguration) {
        setProperty("color", colorConfiguration);
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