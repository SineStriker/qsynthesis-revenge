#ifndef CHORUSKIT_DSPXNOTEENTITY_H
#define CHORUSKIT_DSPXNOTEENTITY_H

#include <QJsonArray>

#include "DspxBaseEntity.h"

namespace Core {

    class DspxPhonemeEntity;
    class DspxPhonemeListEntity;
    class DspxPhonemeInfoEntity;
    class DspxVibratoInfoEntity;
    class DspxNoteEntity;
    class DspxNoteListEntity;

    //===========================================================================
    // Phoneme
    class DspxPhonemeEntityPrivate;

    class CORE_EXPORT DspxPhonemeEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxPhonemeEntity)
        Q_PROPERTY(DspxPhonemeEntity::Type phonemeType READ phonemeType WRITE setPhonemeType NOTIFY phonemeTypeChanged)
        Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)
        Q_PROPERTY(int start READ start WRITE setStart NOTIFY startChanged)
    public:
        explicit DspxPhonemeEntity(QObject *parent = nullptr);
        ~DspxPhonemeEntity();

    public:
        enum Type {
            Ahead,
            Normal,
            Final,
        };
        Q_ENUM(Type)

        Type phonemeType() const;
        void setPhonemeType(Type type);

        QString token() const;
        void setToken(const QString &token);

        int start() const;
        void setStart(int start);

    Q_SIGNALS:
        void phonemeTypeChanged(Type type);
        void tokenChanged(const QString &token);
        void startChanged(int start);

    protected:
        DspxPhonemeEntity(DspxPhonemeEntityPrivate &d, QObject *parent = nullptr);
    };
    //===========================================================================

    //===========================================================================
    // PhonemeList
    class CORE_EXPORT DspxPhonemeListEntity : public AceTreeEntityVector,
                                              public AceTreeEntityVectorHelper<DspxPhonemeEntity> {
        Q_OBJECT
    public:
        explicit DspxPhonemeListEntity(QObject *parent = nullptr);
        ~DspxPhonemeListEntity();

    Q_SIGNALS:
        ACE_TREE_DECLARE_VECTOR_SIGNALS(DspxPhonemeEntity)
    };
    //===========================================================================

    //===========================================================================
    // PhonemeInfo
    class DspxPhonemeInfoEntityPrivate;

    class CORE_EXPORT DspxPhonemeInfoEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxPhonemeInfoEntity)
        Q_PROPERTY(QJsonArray original READ original WRITE setOriginal NOTIFY originalChanged)
    public:
        explicit DspxPhonemeInfoEntity(QObject *parent = nullptr);
        ~DspxPhonemeInfoEntity();

    public:
        QJsonArray original() const;
        void setOriginal(const QJsonArray &original);

        DspxPhonemeListEntity *edited() const;

    Q_SIGNALS:
        void originalChanged(const QJsonArray &org);

    protected:
        DspxPhonemeInfoEntity(DspxPhonemeInfoEntityPrivate &d, QObject *parent = nullptr);
    };
    //===========================================================================

    //===========================================================================
    // VibratoInfo
    class DspxVibratoInfoEntityPrivate;

    class CORE_EXPORT DspxVibratoInfoEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxVibratoInfoEntity)
        Q_PROPERTY(double start READ start WRITE setStart NOTIFY startChanged)
        Q_PROPERTY(double end READ end WRITE setEnd NOTIFY endChanged)
        Q_PROPERTY(double frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
        Q_PROPERTY(double phase READ phase WRITE setPhase NOTIFY phaseChanged)
        Q_PROPERTY(double amplitude READ amplitude WRITE setAmplitude NOTIFY amplitudeChanged)
        Q_PROPERTY(double offset READ offset WRITE setOffset NOTIFY offsetChanged)
    public:
        explicit DspxVibratoInfoEntity(QObject *parent = nullptr);
        ~DspxVibratoInfoEntity();

    public:
        double start() const;
        void setStart(double start);

        double end() const;
        void setEnd(double end);

        double frequency() const;
        void setFrequency(double frequency);

        double phase() const;
        void setPhase(double phase);

        double amplitude() const;
        void setAmplitude(double amplitude);

        double offset() const;
        void setOffset(double offset);

        DspxDoublePointListEntity *points() const;

    Q_SIGNALS:
        void startChanged(double start);
        void endChanged(double end);
        void frequencyChanged(double frequency);
        void phaseChanged(double phase);
        void amplitudeChanged(double amplitude);
        void offsetChanged(double offset);
    };
    //===========================================================================

    //===========================================================================
    // Note
    class DspxNoteEntityPrivate;

    class CORE_EXPORT DspxNoteEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxNoteEntity)
        Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
        Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged)
        Q_PROPERTY(int keyNumber READ keyNumber WRITE setKeyNumber NOTIFY keyNumberChanged)
        Q_PROPERTY(QString lyric READ lyric WRITE setLyric NOTIFY lyricChanged)
    public:
        explicit DspxNoteEntity(QObject *parent = nullptr);
        ~DspxNoteEntity();

    public:
        int position() const;
        void setPosition(int position);

        int length() const;
        void setLength(int length);

        int keyNumber() const;
        void setKeyNumber(int keyNumber);

        QString lyric() const;
        void setLyric(const QString &lyric);

        DspxPhonemeInfoEntity *phonemes() const;
        DspxVibratoInfoEntity *vibrato() const;

    Q_SIGNALS:
        void positionChanged(int position);
        void lengthChanged(int length);
        void keyNumberChanged(int keyNumber);
        void lyricChanged(const QString &lyric);
    };
    //===========================================================================

    //===========================================================================
    // NoteList
    class CORE_EXPORT DspxNoteListEntity : public AceTreeEntityRecordTable,
                                           public AceTreeEntityRecordTableHelper<DspxNoteEntity> {
        Q_OBJECT
    public:
        explicit DspxNoteListEntity(QObject *parent = nullptr);
        ~DspxNoteListEntity();

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;

    Q_SIGNALS:
        ACE_TREE_DECLARE_RECORD_TABLE_SIGNALS(DspxNoteEntity)
    };
    //===========================================================================
}


#endif // CHORUSKIT_DSPXNOTEENTITY_H
