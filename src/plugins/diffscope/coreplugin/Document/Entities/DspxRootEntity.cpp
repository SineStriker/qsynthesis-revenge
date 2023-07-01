#include "DspxRootEntity.h"

#include "AceTreeMacros.h"

#include "../DspxConst.h"

namespace Core {

#define SCHEMA(X) AceTreeStandardSchema::globalSchemaRef(&Dspx##X##Entity ::staticMetaObject)
#define ADD_ELEMENT(L, KEY, X)                                                                                         \
    L.setElementBuilder(KEY, {&Dspx##X##Entity::staticMetaObject, []() { return new Dspx##X##Entity(); }})
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

#define ADD_PROPERTY(L, KEY, V) L.setAttributeSpec(KEY, V)
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
            SET_RECORD2(IntPoint);    // IntPointList
            SET_RECORD2(DoublePoint); // DoublePointList
            SET_RECORD2(AnchorPoint); // AnchorPointList
        }

        // Content
        auto &content = SCHEMA(Content);
        ADD_ELEMENT(content, "metadata", FileMeta);
        ADD_ELEMENT(content, "master", Master);
        ADD_ELEMENT(content, "timeline", Timeline);
        ADD_ELEMENT(content, "tracks", TrackList);

        // FileMeta
        auto &metadata = SCHEMA(FileMeta);
        ADD_PROPERTY(metadata, "author", Dspx::DefaultAuthor);
        ADD_PROPERTY(metadata, "name", Dspx::DefaultProjectName);

        // Master
        {
            auto &master = SCHEMA(Master);
            ADD_ELEMENT(master, "control", BusControl);
        }

        // Timeline
        {
            SET_RECORD2(TimeSignature); // TimeSignature List
            SET_RECORD2(Tempo);         // Tempo List
            SET_RECORD2(TimelineLabel); // TimelineLabel List

            auto &timeline = SCHEMA(Timeline);
            ADD_ELEMENT(timeline, "timeSignatures", TimeSignatureList);
            ADD_ELEMENT(timeline, "tempos", TempoList);
            ADD_ELEMENT(timeline, "labels", TimelineLabelList);

            auto &timeSignature = SCHEMA(TimeSignature);
            ADD_PROPERTY(timeSignature, "pos", Dspx::DefaultTimeSignaturePos);
            ADD_PROPERTY(timeSignature, "num", Dspx::DefaultTimeSignatureNumerator);
            ADD_PROPERTY(timeSignature, "den", Dspx::DefaultTimeSignatureDenominator);

            auto &tempo = SCHEMA(Tempo);
            ADD_PROPERTY(tempo, "pos", Dspx::DefaultTempoPos);
            ADD_PROPERTY(tempo, "value", Dspx::DefaultTempoValue);

            auto &label = SCHEMA(TimelineLabel);
            ADD_PROPERTY(label, "pos", Dspx::DefaultLabelPos);
            ADD_PROPERTY(label, "text", Dspx::DefaultLabelText);
        }

        // Tracks
        {
            auto &track = SCHEMA(Track);
            SET_ROW2(Track); // Track List
            ADD_PROPERTY(track, "name", Dspx::DefaultTrackName);
            ADD_PROPERTY(track, "color", QJsonValue::Object);
            ADD_ELEMENT(track, "control", TrackControl);
            ADD_ELEMENT(track, "clips", ClipList);

            auto &clipTime = SCHEMA(ClipTime);
            ADD_PROPERTY(clipTime, "start", Dspx::DefaultClipStart);
            ADD_PROPERTY(clipTime, "length", Dspx::DefaultClipLength);
            ADD_PROPERTY(clipTime, "clipStart", Dspx::DefaultClipClipStart);
            ADD_PROPERTY(clipTime, "clipLen", Dspx::DefaultClipClipLength);

            auto &clip = SCHEMA(Clip);
            ADD_PROPERTY(clip, "name", Dspx::DefaultClipName);
            ADD_ELEMENT(clip, "time", ClipTime);
            ADD_ELEMENT(clip, "control", BusControl);

            auto &clips = SCHEMA(ClipList); // Clip List
            clips.setRecordTypeKey("type");
            ADD_RECORD(clips, "audio", AudioClip);
            ADD_RECORD(clips, "singing", SingingClip);

            // SingingClip
            {
                auto &singingClip = SCHEMA(SingingClip);
                ADD_ELEMENT(singingClip, "notes", NoteList);
                ADD_ELEMENT(singingClip, "params", ParamSet);

                // Note
                {
                    SET_RECORD2(Note); // Note List

                    auto &note = SCHEMA(Note);
                    ADD_PROPERTY(note, "pos", Dspx::DefaultNotePos);
                    ADD_PROPERTY(note, "length", Dspx::DefaultNoteLength);
                    ADD_PROPERTY(note, "keyNum", Dspx::DefaultNoteKeyNum);
                    ADD_PROPERTY(note, "lyric", Dspx::DefaultNoteLyric);
                    ADD_ELEMENT(note, "vibrato", VibratoInfo);
                    ADD_ELEMENT(note, "phonemes", PhonemeInfo);

                    // VibratoInfo
                    {
                        auto &vibrato = SCHEMA(VibratoInfo);
                        ADD_PROPERTY(vibrato, "start", Dspx::DefaultVibratoStart);
                        ADD_PROPERTY(vibrato, "end", Dspx::DefaultVibratoEnd);
                        ADD_PROPERTY(vibrato, "freq", Dspx::DefaultVibratoFrequency);
                        ADD_PROPERTY(vibrato, "phase", Dspx::DefaultVibratoPhase);
                        ADD_PROPERTY(vibrato, "amp", Dspx::DefaultVibratoAmplitude);
                        ADD_PROPERTY(vibrato, "offset", Dspx::DefaultVibratoOffset);
                        ADD_ELEMENT(vibrato, "points", DoublePointList);
                    }

                    // PhonemeInfo
                    {
                        auto &phonemes = SCHEMA(PhonemeInfo);
                        ADD_DY_DATA(phonemes, "original", QJsonValue::Array);
                        ADD_ELEMENT(phonemes, "edited", PhonemeList);

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
                    ADD_ELEMENT(paramSet, "pitch", ParamInfo); // ParamCurve: Pitch

                    auto &paramInfo = SCHEMA(ParamInfo);
                    ADD_DY_DATA(paramInfo, "original", QJsonValue::Array);
                    ADD_ELEMENT(paramInfo, "edited", ParamCurveList);

                    auto &envParamInfo = SCHEMA(EnvParamInfo);
                    ADD_ELEMENT(envParamInfo, "envelope", ParamCurveList);

                    auto &params = SCHEMA(ParamCurveList); // ParamCurve List
                    params.setRecordTypeKey("type");
                    ADD_RECORD(params, "free", ParamFree);
                    ADD_RECORD(params, "anchor", ParamAnchor);

                    auto &curve = SCHEMA(ParamCurve);
                    ADD_PROPERTY(curve, "start", Dspx::DefaultParamCurveStart);

                    // Free
                    {
                        auto &free = SCHEMA(ParamFree);
                        ADD_PROPERTY(free, "step", Dspx::DefaultParamFreeStep);
                        ADD_ELEMENT(free, "values", ParamFreeData);
                    }

                    // Anchor
                    {
                        auto &anchor = SCHEMA(ParamAnchor);
                        ADD_ELEMENT(anchor, "nodes", AnchorPointList);
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

    //===========================================================================
    // Content
    class DspxContentEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxContentEntityExtra() : metadata(nullptr), master(nullptr), timeline(nullptr), tracks(nullptr) {
        }

        void setup(AceTreeEntity *entity) override {
            Q_UNUSED(entity)
            addChildPointer("metadata", metadata);
            addChildPointer("master", master);
            addChildPointer("timeline", timeline);
            addChildPointer("tracks", tracks);
        }

        DspxFileMetaEntity *metadata;
        DspxMasterEntity *master;
        DspxTimelineEntity *timeline;
        DspxTrackListEntity *tracks;
    };
    DspxContentEntity::DspxContentEntity(QObject *parent) : AceTreeEntityMapping(new DspxContentEntityExtra(), parent) {
    }
    DspxContentEntity::~DspxContentEntity() {
    }
    DspxFileMetaEntity *DspxContentEntity::metadata() const {
        return static_cast<DspxContentEntityExtra *>(extra())->metadata;
    }
    DspxMasterEntity *DspxContentEntity::master() const {
        return static_cast<DspxContentEntityExtra *>(extra())->master;
    }
    DspxTimelineEntity *DspxContentEntity::timeline() const {
        return static_cast<DspxContentEntityExtra *>(extra())->timeline;
    }
    DspxTrackListEntity *DspxContentEntity::tracks() const {
        return static_cast<DspxContentEntityExtra *>(extra())->tracks;
    }
    //===========================================================================

    //===========================================================================
    // FileMeta
    class DspxFileMetaEntityExtra : public AceTreeEntityMappingExtra {
    public:
        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxFileMetaEntity *>(entity);

            addPropertyNotifier("author", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->authorChanged(value.toString());
            });
            addPropertyNotifier("name", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->projectNameChanged(value.toString());
            });
        }
    };
    DspxFileMetaEntity::DspxFileMetaEntity(QObject *parent)
        : AceTreeEntityMapping(new DspxFileMetaEntityExtra(), parent) {
    }
    DspxFileMetaEntity::~DspxFileMetaEntity() {
    }
    QString DspxFileMetaEntity::author() const {
        return attribute("author").toString();
    }
    void DspxFileMetaEntity::setAuthor(const QString &author) {
        setAttribute("author", author);
    }
    QString DspxFileMetaEntity::projectName() const {
        return attribute("name").toString();
    }
    void DspxFileMetaEntity::setProjectName(const QString &projectName) {
        setAttribute("name", projectName);
    }
    //===========================================================================

    //===========================================================================
    // Master
    class DspxMasterEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxMasterEntityExtra() : control(nullptr) {
        }

        void setup(AceTreeEntity *entity) override {
            Q_UNUSED(entity)
            addChildPointer("control", control);
        }

        DspxBusControlEntity *control;
    };
    DspxMasterEntity::DspxMasterEntity(QObject *parent) : AceTreeEntityMapping(new DspxMasterEntityExtra(), parent) {
    }
    DspxMasterEntity::~DspxMasterEntity() {
    }
    DspxBusControlEntity *DspxMasterEntity::control() const {
        return static_cast<DspxMasterEntityExtra *>(extra())->control;
    }
    //===========================================================================

}