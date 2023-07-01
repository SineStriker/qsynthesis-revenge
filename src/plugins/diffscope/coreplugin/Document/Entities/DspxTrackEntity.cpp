#include "DspxTrackEntity.h"
#include "DspxEntityUtils_p.h"

#include "AceTreeMacros.h"

namespace Core {

    //===========================================================================
    // ClipTime
    class DspxClipTimeEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxClipTimeEntityExtra() {
        }
        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxClipTimeEntity *>(entity);

            addPropertyNotifier("start", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->startChanged(value.toInt());
            });
            addPropertyNotifier("length", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->lengthChanged(value.toInt());
            });
            addPropertyNotifier("clipStart", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->clipStartChanged(value.toInt());
            });
            addPropertyNotifier("clipLen", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->clipLengthChanged(value.toInt());
            });
        }
    };
    DspxClipTimeEntity::DspxClipTimeEntity(QObject *parent)
        : AceTreeEntityMapping(new DspxClipTimeEntityExtra(), parent) {
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
    class DspxClipEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxClipEntityExtra(DspxClipEntity::Type type) : type(type), time(nullptr), control(nullptr) {
        }
        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxClipEntity *>(entity);

            addChildPointer("time", time);
            addChildPointer("control", control);

            addPropertyNotifier("name", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->clipNameChanged(value.toString());
            });
        }
        DspxClipEntity::Type type;

        DspxClipTimeEntity *time;
        DspxBusControlEntity *control;
    };

    DspxClipEntity::~DspxClipEntity() {
    }
    DspxClipEntity::Type DspxClipEntity::clipType() const {
        return static_cast<DspxClipEntityExtra *>(extra())->type;
    }
    QString DspxClipEntity::clipName() const {
        return attribute("name").toString();
    }
    void DspxClipEntity::setClipName(const QString &clipName) {
        setAttribute("name", clipName);
    }
    DspxClipTimeEntity *DspxClipEntity::time() const {
        return static_cast<DspxClipEntityExtra *>(extra())->time;
    }
    DspxBusControlEntity *DspxClipEntity::control() const {
        return static_cast<DspxClipEntityExtra *>(extra())->control;
    }
    DspxClipEntity::DspxClipEntity(DspxClipEntityExtra *extra, QObject *parent) : AceTreeEntityMapping(extra, parent) {
    }
    //===========================================================================

    //===========================================================================
    // AudioClip
    class DspxAudioClipEntityExtra : public DspxClipEntityExtra {
    public:
        DspxAudioClipEntityExtra() : DspxClipEntityExtra(DspxClipEntity::Audio) {
        }

        void setup(AceTreeEntity *entity) override {
            DspxClipEntityExtra::setup(entity);

            auto q = static_cast<DspxAudioClipEntity *>(entity);
            addPropertyNotifier("path", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->pathChanged(value.toString());
            });
        }
    };
    DspxAudioClipEntity::DspxAudioClipEntity(QObject *parent) : DspxClipEntity(new DspxAudioClipEntityExtra(), parent) {
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
    class DspxSingingClipEntityExtra : public DspxClipEntityExtra {
    public:
        DspxSingingClipEntityExtra() : DspxClipEntityExtra(DspxClipEntity::Singing), notes(nullptr), params(nullptr) {
        }

        void setup(AceTreeEntity *entity) override {
            DspxClipEntityExtra::setup(entity);

            addChildPointer("notes", notes);
            addChildPointer("params", params);
        }

        DspxNoteListEntity *notes;
        DspxParamSetEntity *params;
    };
    DspxSingingClipEntity::DspxSingingClipEntity(QObject *parent)
        : DspxClipEntity(new DspxSingingClipEntityExtra(), parent) {
    }
    DspxSingingClipEntity::~DspxSingingClipEntity() {
    }
    DspxParamSetEntity *DspxSingingClipEntity::params() const {
        return static_cast<DspxSingingClipEntityExtra *>(extra())->params;
    }
    DspxNoteListEntity *DspxSingingClipEntity::notes() const {
        return static_cast<DspxSingingClipEntityExtra *>(extra())->notes;
    }
    //===========================================================================

    //===========================================================================
    // Clip List
    DspxClipListEntity::DspxClipListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
    }
    DspxClipListEntity::~DspxClipListEntity() {
    }
    void DspxClipListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), [](const AceTreeEntity *left, const AceTreeEntity *right) {
            return reinterpret_cast<const DspxClipEntity *>(left)->time()->start() <
                   reinterpret_cast<const DspxClipEntity *>(right)->time()->start();
        });
    }
    //===========================================================================

    //===========================================================================
    // Track
    class DspxTrackEntityExtra : public AceTreeEntityMappingExtra {
    public:
        DspxTrackEntityExtra() : control(nullptr), clips(nullptr) {
        }

        void setup(AceTreeEntity *entity) override {
            auto q = static_cast<DspxTrackEntity *>(entity);

            addChildPointer("control", control);
            addChildPointer("clips", clips);

            addPropertyNotifier("name", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->trackNameChanged(value.toString());
            });
            addPropertyNotifier("color", [q](ACE_A) {
                Q_UNUSED(oldValue)
                emit q->colorConfigurationChanged(value.toJsonObject());
            });
        }

        DspxTrackControlEntity *control;
        DspxClipListEntity *clips;
    };
    DspxTrackEntity::DspxTrackEntity(QObject *parent) : AceTreeEntityMapping(new DspxTrackEntityExtra(), parent) {
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
        return static_cast<DspxTrackEntityExtra *>(extra())->control;
    }
    DspxClipListEntity *DspxTrackEntity::clips() const {
        return static_cast<DspxTrackEntityExtra *>(extra())->clips;
    }
    //===========================================================================

    //===========================================================================
    // Track List
    DspxTrackListEntity::DspxTrackListEntity(QObject *parent) : AceTreeEntityVector(parent) {
    }
    DspxTrackListEntity::~DspxTrackListEntity() {
    }
    //===========================================================================

}