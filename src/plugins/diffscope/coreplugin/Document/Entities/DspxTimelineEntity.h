#ifndef CHORUSKIT_DSPXTIMELINEENTITY_H
#define CHORUSKIT_DSPXTIMELINEENTITY_H

#include <AceTreeObjectEntity.h>
#include <AceTreeStandardEntity.h>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class DspxTimelineEntity;
    class DspxTimeSignatureEntity;
    class DspxTimeSignatureListEntity;
    class DspxTempoEntity;
    class DspxTempoListEntity;
    class DspxTimeSignatureEntity;
    class DspxTimelineLabelEntity;
    class DspxTimelineLabelListEntity;

    //===========================================================================
    // Timeline
    class DspxTimelineEntityPrivate;

    class CORE_EXPORT DspxTimelineEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxTimelineEntity)
    public:
        explicit DspxTimelineEntity(QObject *parent = nullptr);
        ~DspxTimelineEntity();

    public:
        DspxTimeSignatureListEntity *timeSignatures() const;
        DspxTempoListEntity *tempos() const;
        DspxTimelineLabelListEntity *labels() const;
    };
    //===========================================================================

    //===========================================================================
    // TimeSignature
    class CORE_EXPORT DspxTimeSignatureEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
        Q_PROPERTY(int numerator READ numerator WRITE setNumerator NOTIFY numeratorChanged)
        Q_PROPERTY(int denominator READ denominator WRITE setPosition NOTIFY denominatorChanged)
    public:
        explicit DspxTimeSignatureEntity(QObject *parent = nullptr);
        ~DspxTimeSignatureEntity();

    public:
        int position() const;
        void setPosition(int position);

        int numerator() const;
        void setNumerator(int numerator);

        int denominator() const;
        void setDenominator(int denominator);

    Q_SIGNALS:
        void positionChanged(int position);
        void numeratorChanged(int numerator);
        void denominatorChanged(int denominator);
    };
    //===========================================================================

    //===========================================================================
    // TimeSignature List
    class CORE_EXPORT DspxTimeSignatureListEntity : public AceTreeEntityRecordTable,
                                                    public AceTreeEntityRecordTableHelper<DspxTimeSignatureEntity> {
        Q_OBJECT
    public:
        explicit DspxTimeSignatureListEntity(QObject *parent = nullptr);
        ~DspxTimeSignatureListEntity();

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;

    Q_SIGNALS:
        ACE_TREE_DECLARE_RECORD_TABLE_SIGNALS(DspxTimeSignatureEntity)
    };
    //===========================================================================

    //===========================================================================
    // Tempo
    class CORE_EXPORT DspxTempoEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
        Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    public:
        explicit DspxTempoEntity(QObject *parent = nullptr);
        ~DspxTempoEntity();

    public:
        int position() const;
        void setPosition(int position);

        double value() const;
        void setValue(double value);

    Q_SIGNALS:
        void positionChanged(int position);
        void valueChanged(double value);
    };
    //===========================================================================

    //===========================================================================
    // Tempo List
    class CORE_EXPORT DspxTempoListEntity : public AceTreeEntityRecordTable,
                                            public AceTreeEntityRecordTableHelper<DspxTempoEntity> {
        Q_OBJECT
    public:
        explicit DspxTempoListEntity(QObject *parent = nullptr);
        ~DspxTempoListEntity();

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;

    Q_SIGNALS:
        ACE_TREE_DECLARE_RECORD_TABLE_SIGNALS(DspxTempoEntity)
    };
    //===========================================================================

    //===========================================================================
    // TimelineLabel
    class CORE_EXPORT DspxTimelineLabelEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
        Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    public:
        explicit DspxTimelineLabelEntity(QObject *parent = nullptr);
        ~DspxTimelineLabelEntity();

    public:
        int position() const;
        void setPosition(int position);

        QString text() const;
        void setText(const QString &text);

    Q_SIGNALS:
        void positionChanged(int position);
        void textChanged(const QString &text);
    };
    //===========================================================================

    //===========================================================================
    // TimelineLabel List
    class CORE_EXPORT DspxTimelineLabelListEntity : public AceTreeEntityRecordTable,
                                                    public AceTreeEntityRecordTableHelper<DspxTimelineLabelEntity> {
        Q_OBJECT
    public:
        explicit DspxTimelineLabelListEntity(QObject *parent = nullptr);
        ~DspxTimelineLabelListEntity();

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;

    Q_SIGNALS:
        ACE_TREE_DECLARE_RECORD_TABLE_SIGNALS(DspxTimelineLabelEntity)
    };
    //===========================================================================

}

#endif // CHORUSKIT_DSPXTIMELINEENTITY_H
