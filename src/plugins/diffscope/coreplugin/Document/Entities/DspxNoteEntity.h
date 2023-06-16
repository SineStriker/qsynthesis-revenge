//
// Created by Functioner on 2023/6/16.
//

#ifndef CHORUSKIT_DSPXNOTEENTITY_H
#define CHORUSKIT_DSPXNOTEENTITY_H

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

    class CORE_EXPORT DspxPhonemeEntity : public AceTreeStandardEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxPhonemeEntity)
    public:
        explicit DspxPhonemeEntity(QObject *parent = nullptr);
        ~DspxPhonemeEntity();

        QString name() const override;

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

        DspxWorkspaceEntity *workspace() const;
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

        QString name() const override;
    };
    //===========================================================================

    //===========================================================================
    // PhonemeInfo
    class DspxPhonemeInfoEntityPrivate;

    class CORE_EXPORT DspxPhonemeInfoEntity : public AceTreeStandardEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxPhonemeInfoEntity)
    public:
        explicit DspxPhonemeInfoEntity(QObject *parent = nullptr);
        ~DspxPhonemeInfoEntity();

        QString name() const override;

    public:
        QJsonArray original() const;
        void setOriginal(const QJsonArray &original);

        DspxPhonemeListEntity *edited() const;
    };
    //===========================================================================

    //===========================================================================
    // VibratoInfo
    class DspxVibratoInfoEntityPrivate;

    class CORE_EXPORT DspxVibratoInfoEntity : public AceTreeStandardEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxVibratoInfoEntity)
    public:
        explicit DspxVibratoInfoEntity(QObject *parent = nullptr);
        ~DspxVibratoInfoEntity();

        QString name() const override;

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
    };
    //===========================================================================

    //===========================================================================
    // Note
    class DspxNoteEntityPrivate;

    class CORE_EXPORT DspxNoteEntity : public AceTreeStandardEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxNoteEntity)
    public:
        explicit DspxNoteEntity(QObject *parent = nullptr);
        ~DspxNoteEntity();

        QString name() const override;

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
        DspxWorkspaceEntity *workspace() const;
    };
    //===========================================================================

    //===========================================================================
    // NoteList
    class CORE_EXPORT DspxNoteListEntity : public AceTreeEntityRecordTable,
                                           public AceTreeEntityRecordTableHelper<DspxPhonemeEntity> {
        Q_OBJECT
    public:
        explicit DspxNoteListEntity(QObject *parent = nullptr);
        ~DspxNoteListEntity();

        QString name() const override;

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;
    };
    //===========================================================================
}


#endif // CHORUSKIT_DSPXNOTEENTITY_H
