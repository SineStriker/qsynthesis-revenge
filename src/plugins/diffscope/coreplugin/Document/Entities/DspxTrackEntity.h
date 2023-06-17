#ifndef CHORUSKIT_DSPXTRACKENTITY_H
#define CHORUSKIT_DSPXTRACKENTITY_H

#include "DspxNoteEntity.h"
#include "DspxParamEntity.h"

namespace Core {

    class DspxClipTimeEntity;
    class DspxClipEntity;
    class DspxAudioClipEntity;
    class DspxSingingClipEntity;
    class DspxClipListEntity;
    class DspxTrackEntity;
    class DspxTrackListEntity;

    //===========================================================================
    // ClipTime
    class CORE_EXPORT DspxClipTimeEntity : public AceTreeEntityMapping {
        Q_OBJECT
    public:
        explicit DspxClipTimeEntity(QObject *parent = nullptr);
        ~DspxClipTimeEntity();

        QString name() const override;

    public:
        int start() const;
        void setStart(int start);

        int length() const;
        void setLength(int length);

        int clipStart() const;
        void setClipStart(int clipStart);

        int clipLength() const;
        void setClipLength(int clipLength);
    };
    //===========================================================================

    //===========================================================================
    // Clip
    class DspxClipEntityPrivate;

    class CORE_EXPORT DspxClipEntity : public AceTreeStandardEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxClipEntity)
    public:
        enum Type {
            Singing,
            Audio,
        };

        explicit DspxClipEntity(Type type, QObject *parent = nullptr);
        ~DspxClipEntity();

    public:
        Type clipType() const;

        QString clipName() const;
        void setClipName(const QString &clipName);

        DspxClipTimeEntity *time() const;
        DspxBusControlEntity *control() const;

    protected:
        DspxClipEntity(DspxClipEntityPrivate &d, QObject *parent = nullptr);
    };
    //===========================================================================

    //===========================================================================
    // AudioClip
    class DspxAudioClipEntityPrivate;

    class CORE_EXPORT DspxAudioClipEntity : public DspxClipEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxAudioClipEntity)
    public:
        explicit DspxAudioClipEntity(QObject *parent = nullptr);
        ~DspxAudioClipEntity();

    public:
        QString path() const;
        void setPath(const QString &path);
    };
    //===========================================================================

    //===========================================================================
    // SingingClip
    class DspxSingingClipEntityPrivate;

    class CORE_EXPORT DspxSingingClipEntity : public DspxClipEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxSingingClipEntity)
    public:
        explicit DspxSingingClipEntity(QObject *parent = nullptr);
        ~DspxSingingClipEntity();

    public:
        DspxParamSetEntity *params() const;
        DspxNoteListEntity *notes() const;
    };
    //===========================================================================

    //===========================================================================
    // Clip List
    class CORE_EXPORT DspxClipListEntity : public AceTreeEntityRecordTable,
                                           public AceTreeEntityRecordTableHelper<DspxClipEntity> {
        Q_OBJECT
    public:
        explicit DspxClipListEntity(QObject *parent = nullptr);
        ~DspxClipListEntity();

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;
    };
    //===========================================================================

    //===========================================================================
    // Track
    class DspxTrackEntityPrivate;

    class CORE_EXPORT DspxTrackEntity : public AceTreeStandardEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxTrackEntity)
    public:
        explicit DspxTrackEntity(QObject *parent = nullptr);
        ~DspxTrackEntity();

    public:
        QString trackName() const;
        void setTrackName(const QString &trackName);

        QJsonObject colorConfiguration() const;
        void setColorConfiguration(const QJsonObject &colorConfiguration);

        DspxTrackControlEntity *control() const;
        DspxClipListEntity *clips() const;
    };
    //===========================================================================

    //===========================================================================
    // Track List
    class CORE_EXPORT DspxTrackListEntity : public AceTreeEntityVector,
                                            public AceTreeEntityVectorHelper<DspxParamCurveEntity> {
        Q_OBJECT
    public:
        explicit DspxTrackListEntity(QObject *parent = nullptr);
        ~DspxTrackListEntity();
    };
    //===========================================================================

}

#endif // CHORUSKIT_DSPXTRACKENTITY_H
