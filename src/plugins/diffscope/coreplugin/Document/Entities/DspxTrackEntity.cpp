#include "DspxTrackEntity.h"
#include "AceTreeStandardEntity_p.h"

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
            time = nullptr;
            control = nullptr;
            workspace = nullptr;
            childPostAssignRefs.insert("time", &time);
            childPostAssignRefs.insert("control", &control);
            childPostAssignRefs.insert("workspace", &workspace);
        }
        ~DspxClipEntityPrivate() = default;

        DspxClipEntity::Type type;

        DspxClipTimeEntity *time;
        DspxBusControlEntity *control;
        DspxWorkspaceEntity *workspace;
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
    DspxWorkspaceEntity *DspxClipEntity::workspace() const {
        Q_D(const DspxClipEntity);
        return d->workspace;
    }
    DspxClipEntity::DspxClipEntity(DspxClipEntityPrivate &d, QObject *parent) : AceTreeStandardEntity(d, parent) {
    }
    //===========================================================================

    //===========================================================================
    // AudioClip
    class DspxAudioClipEntityPrivate : public DspxClipEntityPrivate {
    public:
        DspxAudioClipEntityPrivate() : DspxClipEntityPrivate(DspxClipEntity::Audio) {
        }
        ~DspxAudioClipEntityPrivate() = default;
    };
    DspxAudioClipEntity::DspxAudioClipEntity(QObject *parent)
        : DspxClipEntity(*new DspxAudioClipEntityPrivate(), parent) {
    }
    DspxAudioClipEntity::~DspxAudioClipEntity() {
    }
    QString DspxAudioClipEntity::name() const {
        return "audio";
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
    QString DspxSingingClipEntity::name() const {
        return "singing";
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
    }
    DspxClipListEntity::~DspxClipListEntity() {
    }
    QString DspxClipListEntity::name() const {
        return "clip";
    }
    void DspxClipListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), [](const AceTreeEntity *left, const AceTreeEntity *right) -> bool {
            return left->property("start").toInt() < right->property("start").toInt();
        });
    }
    //===========================================================================

    //===========================================================================
    // Track
    class DspxTrackEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxTrackEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
            control = nullptr;
            clips = nullptr;
            workspace = nullptr;
            childPostAssignRefs.insert("control", &control);
            childPostAssignRefs.insert("clips", &clips);
            childPostAssignRefs.insert("workspace", &workspace);
        }
        ~DspxTrackEntityPrivate() = default;

        DspxTrackControlEntity *control;
        DspxClipListEntity *clips;
        DspxWorkspaceEntity *workspace;
    };
    DspxTrackEntity::DspxTrackEntity(QObject *parent) : AceTreeStandardEntity(*new DspxTrackEntityPrivate(), parent) {
    }
    DspxTrackEntity::~DspxTrackEntity() {
    }
    QString DspxTrackEntity::name() const {
        return "track";
    }
    QString DspxTrackEntity::trackName() const {
        return property("name").toString();
    }
    void DspxTrackEntity::setTrackName(const QString &trackName) {
        setProperty("name", trackName);
    }
    DspxTrackControlEntity *DspxTrackEntity::control() const {
        Q_D(const DspxTrackEntity);
        return d->control;
    }
    DspxClipListEntity *DspxTrackEntity::clips() const {
        Q_D(const DspxTrackEntity);
        return d->clips;
    }
    DspxWorkspaceEntity *DspxTrackEntity::workspace() const {
        Q_D(const DspxTrackEntity);
        return d->workspace;
    }
    //===========================================================================

    //===========================================================================
    // Track List
    DspxTrackListEntity::DspxTrackListEntity(QObject *parent) : AceTreeEntityVector(parent) {
    }
    DspxTrackListEntity::~DspxTrackListEntity() {
    }
    QString DspxTrackListEntity::name() const {
        return "tracks";
    }
    //===========================================================================

}