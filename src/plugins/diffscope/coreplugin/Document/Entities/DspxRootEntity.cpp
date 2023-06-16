#include "DspxRootEntity.h"
#include "DspxRootEntity_p.h"

#include "../DspxConst.h"

namespace Core {

#define SCHEMA(X) AceTreeStandardSchema::globalSchemaRef(&Dspx##X##Entity ::staticMetaObject)
#define ADD_NODE(L, KEY, X)                                                                                            \
    L.setNodeBuilder(KEY, {&Dspx##X##Entity::staticMetaObject, []() { return new Dspx##X##Entity(); }})
#define ADD_ROW(L, KEY, X)                                                                                             \
    L.setRowBuilder(KEY, {&Dspx##X##Entity::staticMetaObject, []() { return new Dspx##X##Entity(); }})
#define ADD_RECORD(L, KEY, X)                                                                                          \
    L.setRecordBuilder(KEY, {&Dspx##X##Entity::staticMetaObject, []() { return new Dspx##X##Entity(); }})

#define SET_ROW(L, X)                                                                                                  \
    L.setRowDefaultBuilder({&Dspx##X##Entity::staticMetaObject, []() { return new Dspx##X##Entity(); }})
#define SET_RECORD(L, X)                                                                                               \
    L.setRecordDefaultBuilder({&Dspx##X##Entity::staticMetaObject, []() { return new Dspx##X##Entity(); }})
#define SET_ROW2(X)    SET_ROW(SCHEMA(X##List), X)
#define SET_RECORD2(X) SET_RECORD(SCHEMA(X##List), X)

#define ADD_PROPERTY(L, KEY, V) L.setPropertySpec(KEY, V)
#define ADD_DY_DATA(L, KEY, V)  L.setDynamicDataSpec(KEY, V)

    void initDspxEntitiesSchema() {
        // Base
        {
            auto &busCtl = SCHEMA(BusControl);
            ADD_PROPERTY(busCtl, "gain", Dspx::DefaultGain);
            ADD_PROPERTY(busCtl, "mute", Dspx::DefaultMute);

            auto &trackCtl = SCHEMA(TrackControl);
            ADD_PROPERTY(trackCtl, "pan", Dspx::DefaultPan);
            ADD_PROPERTY(trackCtl, "solo", Dspx::DefaultSolo);
        }

        {
            auto &intPoint = SCHEMA(IntPoint);
            SET_RECORD2(IntPoint); // IntPointList
            ADD_PROPERTY(intPoint, "x", Dspx::DefaultParamAnchorNodeX);
            ADD_PROPERTY(intPoint, "y", Dspx::DefaultParamAnchorNodeY);

            auto &doublePoint = SCHEMA(DoublePoint);
            SET_RECORD2(DoublePoint); // DoublePointList
            ADD_PROPERTY(doublePoint, "x", Dspx::DefaultVibratoPointX);
            ADD_PROPERTY(doublePoint, "y", Dspx::DefaultVibratoPointX);

            auto &anchorPoint = SCHEMA(AnchorPoint);
            SET_RECORD2(AnchorPoint); // AnchorPointList
            ADD_PROPERTY(anchorPoint, "interp", Dspx::DefaultParamAnchorNodeInterpolation);
        }

        // Root
        auto &root = SCHEMA(Root);
        ADD_NODE(root, "metadata", Metadata);
        ADD_NODE(root, "content", Content);
        ADD_NODE(root, "workspace", Workspace);

        // Metadata
        auto &metadata = SCHEMA(Metadata);
        ADD_PROPERTY(metadata, "version", Dspx::Version);
        ADD_PROPERTY(metadata, "author", Dspx::DefaultAuthor);
        ADD_PROPERTY(metadata, "name", Dspx::DefaultProjectName);

        // Content
        auto &content = SCHEMA(Content);
        ADD_NODE(content, "master", Master);
        ADD_NODE(content, "timeline", Timeline);
        ADD_NODE(content, "tracks", TrackList);
        ADD_NODE(content, "workspace", Workspace);

        // Master
        {
            auto &master = SCHEMA(Master);
            ADD_NODE(master, "control", BusControl);
        }

        // Timeline
        {
            auto &timeline = SCHEMA(Timeline);
            ADD_NODE(timeline, "timeSignatures", TimeSignatureList);
            ADD_NODE(timeline, "tempos", TempoList);
            ADD_NODE(timeline, "labels", TimelineLabelList);

            auto &timeSignature = SCHEMA(TimeSignature);
            SET_RECORD2(TimeSignature); // TimeSignature List
            ADD_PROPERTY(timeSignature, "pos", Dspx::DefaultTimeSignaturePos);
            ADD_PROPERTY(timeSignature, "numerator", Dspx::DefaultTimeSignatureNumerator);
            ADD_PROPERTY(timeSignature, "denominator", Dspx::DefaultTimeSignatureDenominator);

            auto &tempo = SCHEMA(Tempo);
            SET_RECORD2(Tempo); // Tempo List
            ADD_PROPERTY(tempo, "pos", Dspx::DefaultTempoPos);
            ADD_PROPERTY(tempo, "value", Dspx::DefaultTempoValue);

            auto &label = SCHEMA(TimelineLabel);
            SET_RECORD2(TimelineLabel); // TimelineLabel List
            ADD_PROPERTY(label, "pos", Dspx::DefaultLabelPos);
            ADD_PROPERTY(label, "text", Dspx::DefaultLabelText);
        }

        // Tracks
        {
            auto &track = SCHEMA(Track);
            SET_ROW2(Track); // Track List
            ADD_PROPERTY(track, "name", Dspx::DefaultTrackName);
            ADD_NODE(track, "control", TrackControl);
            ADD_NODE(track, "workspace", Workspace);
            ADD_NODE(track, "clips", ClipList);

            auto &clipTime = SCHEMA(ClipTime);
            ADD_PROPERTY(clipTime, "start", Dspx::DefaultClipStart);
            ADD_PROPERTY(clipTime, "length", Dspx::DefaultClipLength);
            ADD_PROPERTY(clipTime, "clipStart", Dspx::DefaultClipClipStart);
            ADD_PROPERTY(clipTime, "clipLen", Dspx::DefaultClipClipLength);

            auto &clip = SCHEMA(Clip);
            ADD_PROPERTY(clip, "name", Dspx::DefaultClipName);
            ADD_NODE(clip, "time", ClipTime);
            ADD_NODE(clip, "control", BusControl);
            ADD_NODE(clip, "workspace", Workspace);

            auto &clips = SCHEMA(ClipList); // Clip List
            ADD_ROW(clips, "audio", AudioClip);
            ADD_ROW(clips, "singing", SingingClip);

            // SingingClip
            {
                auto &singingClip = SCHEMA(SingingClip);
                ADD_NODE(singingClip, "notes", NoteList);
                ADD_NODE(singingClip, "params", ParamSet);

                // Note
                {
                    auto &note = SCHEMA(Note);
                    SET_RECORD2(Note); // Note List
                    ADD_PROPERTY(note, "pos", Dspx::DefaultNotePos);
                    ADD_PROPERTY(note, "length", Dspx::DefaultNoteLength);
                    ADD_PROPERTY(note, "keyNum", Dspx::DefaultNoteKeyNum);
                    ADD_PROPERTY(note, "lyric", Dspx::DefaultNoteLyric);
                    ADD_NODE(note, "vibrato", VibratoInfo);
                    ADD_NODE(note, "phonemes", PhonemeInfo);
                    ADD_NODE(note, "workspace", Workspace);

                    // VibratoInfo
                    {
                        auto &vibrato = SCHEMA(VibratoInfo);
                        ADD_PROPERTY(vibrato, "start", Dspx::DefaultVibratoStart);
                        ADD_PROPERTY(vibrato, "end", Dspx::DefaultVibratoEnd);
                        ADD_PROPERTY(vibrato, "freq", Dspx::DefaultVibratoFrequency);
                        ADD_PROPERTY(vibrato, "phase", Dspx::DefaultVibratoPhase);
                        ADD_PROPERTY(vibrato, "amp", Dspx::DefaultVibratoAmplitude);
                        ADD_PROPERTY(vibrato, "offset", Dspx::DefaultVibratoOffset);
                        ADD_NODE(vibrato, "points", DoublePointList);
                    }

                    // PhonemeInfo
                    {
                        auto &phonemes = SCHEMA(PhonemeInfo);
                        ADD_DY_DATA(phonemes, "original", QJsonValue::Array);
                        ADD_NODE(phonemes, "edited", PhonemeList);

                        auto &phoneme = SCHEMA(Phoneme);
                        SET_ROW2(Phoneme); // Phoneme List
                        ADD_PROPERTY(phoneme, "type", Dspx::DefaultPhonemeType);
                        ADD_PROPERTY(phoneme, "token", Dspx::DefaultPhonemeToken);
                        ADD_PROPERTY(phoneme, "start", Dspx::DefaultClipStart);
                    }
                }

                // ParamInfo
                {
                    auto &paramSet = SCHEMA(ParamSet);
                    ADD_NODE(paramSet, "pitch", ParamInfo); // ParamCurve: Pitch

                    auto &paramInfo = SCHEMA(ParamInfo);
                    ADD_PROPERTY(paramInfo, "original", QJsonValue::Array);
                    ADD_NODE(paramInfo, "edited", ParamCurveList);
                    ADD_NODE(paramInfo, "envelope", ParamCurveList);

                    auto &params = SCHEMA(ParamCurveList); // ParamCurve List
                    ADD_RECORD(params, "free", ParamFree);
                    ADD_RECORD(params, "anchor", ParamAnchor);

                    // Free
                    {
                        auto &free = SCHEMA(ParamFree);
                        ADD_PROPERTY(free, "start", Dspx::DefaultParamFreeStart);
                        ADD_PROPERTY(free, "step", Dspx::DefaultParamFreeStep);
                        ADD_NODE(free, "values", ParamFreeData);
                    }

                    // Anchor
                    {
                        auto &anchor = SCHEMA(ParamAnchor);
                        ADD_NODE(anchor, "nodes", IntPointList);
                    }
                }
            }

            // AudioClip
            {
                auto &audioClip = SCHEMA(AudioClip);
                ADD_PROPERTY(audioClip, "path", Dspx::DefaultAudioClipPath);
            }
        }
    }

    //===========================================================================
    // Metadata
    DspxMetadataEntity::DspxMetadataEntity(QObject *parent) : AceTreeEntityMapping(parent) {
    }

    DspxMetadataEntity::~DspxMetadataEntity() {
    }

    QString DspxMetadataEntity::name() const {
        return "metadata";
    }
    //===========================================================================

    //===========================================================================
    // Root
    DspxRootEntityPrivate::DspxRootEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
        metadata = nullptr;
        content = nullptr;
        workspace = nullptr;
        childPostAssignRefs.insert("metadata", &metadata);
        childPostAssignRefs.insert("content", &content);
        childPostAssignRefs.insert("workspace", &workspace);
    }

    DspxRootEntityPrivate::~DspxRootEntityPrivate() {
    }

    DspxRootEntity::DspxRootEntity(QObject *parent) : AceTreeStandardEntity(*new DspxRootEntityPrivate(), parent) {
    }

    DspxRootEntity::~DspxRootEntity() {
    }

    QString DspxRootEntity::name() const {
        return "root";
    }

    QString DspxRootEntity::version() const {
        Q_D(const DspxRootEntity);
        return d->metadata->property("version").toString();
    }

    void DspxRootEntity::setVersion(const QString &version) {
        Q_D(DspxRootEntity);
        d->metadata->setProperty("version", version);
    }

    QString DspxRootEntity::author() const {
        Q_D(const DspxRootEntity);
        return d->metadata->property("author").toString();
    }

    void DspxRootEntity::setAuthor(const QString &author) {
        Q_D(DspxRootEntity);
        d->metadata->setProperty("author", author);
    }

    QString DspxRootEntity::projectName() const {
        Q_D(const DspxRootEntity);
        return d->metadata->property("name").toString();
    }

    void DspxRootEntity::setProjectName(const QString &projectName) {
        Q_D(DspxRootEntity);
        d->metadata->setProperty("name", projectName);
    }

    DspxWorkspaceEntity *DspxRootEntity::workspace() const {
        Q_D(const DspxRootEntity);
        return d->workspace;
    }

    DspxContentEntity *DspxRootEntity::content() const {
        Q_D(const DspxRootEntity);
        return d->content;
    }
    //===========================================================================

    //===========================================================================
    // Content
    DspxContentEntityPrivate::DspxContentEntityPrivate()
        : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
        master = nullptr;
        timeline = nullptr;
        tracks = nullptr;
        workspace = nullptr;
        childPostAssignRefs.insert("master", &master);
        childPostAssignRefs.insert("timeline", &timeline);
        childPostAssignRefs.insert("tracks", &tracks);
        childPostAssignRefs.insert("workspace", &workspace);
    }
    DspxContentEntityPrivate::~DspxContentEntityPrivate() {
    }
    DspxContentEntity::DspxContentEntity(QObject *parent)
        : AceTreeStandardEntity(*new DspxContentEntityPrivate(), parent) {
    }
    DspxContentEntity::~DspxContentEntity() {
    }
    QString DspxContentEntity::name() const {
        return "content";
    }
    DspxMasterEntity *DspxContentEntity::master() const {
        Q_D(const DspxContentEntity);
        return d->master;
    }
    DspxTimelineEntity *DspxContentEntity::timeline() const {
        Q_D(const DspxContentEntity);
        return d->timeline;
    }
    DspxTrackListEntity *DspxContentEntity::tracks() const {
        Q_D(const DspxContentEntity);
        return d->tracks;
    }
    DspxWorkspaceEntity *DspxContentEntity::workspace() const {
        Q_D(const DspxContentEntity);
        return d->workspace;
    }
    //===========================================================================

    //===========================================================================
    // Master
    DspxMasterEntityPrivate::DspxMasterEntityPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
        control = nullptr;
        childPostAssignRefs.insert("control", &control);
    }
    DspxMasterEntityPrivate::~DspxMasterEntityPrivate() {
    }
    DspxMasterEntity::DspxMasterEntity(QObject *parent)
        : AceTreeStandardEntity(*new DspxMasterEntityPrivate(), parent) {
    }
    DspxMasterEntity::~DspxMasterEntity() {
    }
    QString DspxMasterEntity::name() const {
        return "master";
    }
    DspxBusControlEntity *DspxMasterEntity::control() const {
        Q_D(const DspxMasterEntity);
        return d->control;
    }
    //===========================================================================

}