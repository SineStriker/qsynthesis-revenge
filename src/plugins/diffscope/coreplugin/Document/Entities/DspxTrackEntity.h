#ifndef DSPXTRACKENTITY_H
#define DSPXTRACKENTITY_H

#include <QJsonObject>

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
        Q_PROPERTY(int start READ start WRITE setStart NOTIFY startChanged)
        Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged)
        Q_PROPERTY(int clipStart READ clipStart WRITE setClipStart NOTIFY clipStartChanged)
        Q_PROPERTY(int clipLength READ clipLength WRITE setClipLength NOTIFY clipLengthChanged)
    public:
        explicit DspxClipTimeEntity(QObject *parent = nullptr);
        ~DspxClipTimeEntity();

    public:
        int start() const;
        void setStart(int start);

        int length() const;
        void setLength(int length);

        int clipStart() const;
        void setClipStart(int clipStart);

        int clipLength() const;
        void setClipLength(int clipLength);

    Q_SIGNALS:
        void startChanged(int start);
        void lengthChanged(int length);
        void clipStartChanged(int clipStart);
        void clipLengthChanged(int clipLength);
    };
    //===========================================================================

    //===========================================================================
    // Clip
    class DspxClipEntityExtra;

    class CORE_EXPORT DspxClipEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_PROPERTY(QString clipName READ clipName WRITE setClipName NOTIFY clipNameChanged)
    public:
        enum Type {
            Singing,
            Audio,
        };
        
        ~DspxClipEntity();

    public:
        Type clipType() const;

        QString clipName() const;
        void setClipName(const QString &clipName);

        DspxClipTimeEntity *time() const;
        DspxBusControlEntity *control() const;

    protected:
        explicit DspxClipEntity(DspxClipEntityExtra *extra, QObject *parent = nullptr);

    Q_SIGNALS:
        void clipNameChanged(const QString &clipName);
    };
    //===========================================================================

    //===========================================================================
    // AudioClip
    class CORE_EXPORT DspxAudioClipEntity : public DspxClipEntity {
        Q_OBJECT
        Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    public:
        explicit DspxAudioClipEntity(QObject *parent = nullptr);
        ~DspxAudioClipEntity();

    public:
        QString path() const;
        void setPath(const QString &path);

    Q_SIGNALS:
        void pathChanged(const QString &path);
    };
    //===========================================================================

    //===========================================================================
    // SingingClip
    class CORE_EXPORT DspxSingingClipEntity : public DspxClipEntity {
        Q_OBJECT
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

    Q_SIGNALS:
        ACE_TREE_DECLARE_RECORD_TABLE_SIGNALS(DspxClipEntity)
    };
    //===========================================================================

    //===========================================================================
    // Track
    class CORE_EXPORT DspxTrackEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_PROPERTY(QString trackName READ trackName WRITE setTrackName NOTIFY trackNameChanged)
        Q_PROPERTY(QJsonObject colorConfiguration READ colorConfiguration WRITE setColorConfiguration NOTIFY
                       colorConfigurationChanged)
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

    Q_SIGNALS:
        void trackNameChanged(const QString &trackName);
        void colorConfigurationChanged(const QJsonObject &colorConfiguration);
    };
    //===========================================================================

    //===========================================================================
    // Track List
    class CORE_EXPORT DspxTrackListEntity : public AceTreeEntityVector,
                                            public AceTreeEntityVectorHelper<DspxTrackEntity> {
        Q_OBJECT
    public:
        explicit DspxTrackListEntity(QObject *parent = nullptr);
        ~DspxTrackListEntity();

    Q_SIGNALS:
        ACE_TREE_DECLARE_VECTOR_SIGNALS(DspxTrackEntity)
    };
    //===========================================================================

}

#endif // DSPXTRACKENTITY_H
