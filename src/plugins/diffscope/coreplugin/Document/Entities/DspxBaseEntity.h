#ifndef CHORUSKIT_DSPXBASEENTITY_H
#define CHORUSKIT_DSPXBASEENTITY_H

#include <AceTreeObjectEntity.h>
#include <AceTreeStandardEntity.h>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class DspxBusControlEntity;
    class DspxTrackControlEntity;
    class DspxIntPointEntity;
    class DspxIntPointListEntity;
    class DspxDoublePointEntity;
    class DspxDoublePointListEntity;
    class DspxAnchorPointEntity;
    class DspxAnchorPointListEntity;

    //===========================================================================
    // BusControl
    class CORE_EXPORT DspxBusControlEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_PROPERTY(double gain READ gain WRITE setGain NOTIFY gainChanged)
        Q_PROPERTY(bool mute READ mute WRITE setMute NOTIFY muteChanged)
    public:
        explicit DspxBusControlEntity(QObject *parent = nullptr);
        ~DspxBusControlEntity();

    public:
        double gain() const;
        void setGain(double gain);

        bool mute() const;
        void setMute(bool mute);

    Q_SIGNALS:
        void gainChanged(double gain);
        void muteChanged(bool mute);
    };
    //===========================================================================

    //===========================================================================
    // TrackControl
    class CORE_EXPORT DspxTrackControlEntity : public DspxBusControlEntity {
        Q_OBJECT
        Q_PROPERTY(double pan READ pan WRITE setPan NOTIFY panChanged)
        Q_PROPERTY(bool solo READ solo WRITE setSolo NOTIFY soloChanged)
    public:
        explicit DspxTrackControlEntity(QObject *parent = nullptr);
        ~DspxTrackControlEntity();

    public:
        double pan() const;
        void setPan(double pan);

        bool solo() const;
        void setSolo(bool solo);

    Q_SIGNALS:
        void panChanged(double pan);
        void soloChanged(bool solo);
    };
    //===========================================================================

    //===========================================================================
    // IntPoint
    class DspxIntPointEntityPrivate;

    class CORE_EXPORT DspxIntPointEntity : public AceTreeEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxIntPointEntity)
        Q_PROPERTY(QPoint pos READ pos WRITE setPos NOTIFY positionChanged)
        Q_PROPERTY(int x READ x)
        Q_PROPERTY(int y READ y)
    public:
        explicit DspxIntPointEntity(QObject *parent = nullptr);
        ~DspxIntPointEntity();

    public:
        bool read(const QJsonValue &value) override;
        QJsonValue write() const override;

        int x() const;
        int y() const;

        QPoint pos() const;
        void setPos(const QPoint &pos);

    protected:
        void doInitialize() override;
        void doSetup() override;

    Q_SIGNALS:
        void positionChanged(int x, int y);

    protected:
        DspxIntPointEntity(DspxIntPointEntityPrivate &d, QObject *parent = nullptr);
    };
    //===========================================================================

    //===========================================================================
    // IntPoint List
    class CORE_EXPORT DspxIntPointListEntity : public AceTreeEntityRecordTable,
                                               public AceTreeEntityRecordTableHelper<DspxIntPointEntity> {
        Q_OBJECT
    public:
        explicit DspxIntPointListEntity(QObject *parent = nullptr);
        ~DspxIntPointListEntity();

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;

    Q_SIGNALS:
        ACE_TREE_DECLARE_RECORD_TABLE_SIGNALS(DspxIntPointEntity)
    };
    //===========================================================================

    //===========================================================================
    // DoublePoint
    class DspxDoublePointEntityPrivate;

    class CORE_EXPORT DspxDoublePointEntity : public AceTreeEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxDoublePointEntity)
        Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY positionChanged)
        Q_PROPERTY(double x READ x)
        Q_PROPERTY(double y READ y)
    public:
        explicit DspxDoublePointEntity(QObject *parent = nullptr);
        ~DspxDoublePointEntity();

    public:
        bool read(const QJsonValue &value) override;
        QJsonValue write() const override;

        double x() const;
        double y() const;

        QPointF pos() const;
        void setPos(const QPointF &pos);

    protected:
        void doInitialize() override;
        void doSetup() override;

    Q_SIGNALS:
        void positionChanged(double x, double y);

    protected:
        DspxDoublePointEntity(DspxDoublePointEntityPrivate &d, QObject *parent = nullptr);
    };
    //===========================================================================

    //===========================================================================
    // DoublePoint List
    class CORE_EXPORT DspxDoublePointListEntity : public AceTreeEntityRecordTable,
                                                  public AceTreeEntityRecordTableHelper<DspxDoublePointEntity> {
        Q_OBJECT
    public:
        explicit DspxDoublePointListEntity(QObject *parent = nullptr);
        ~DspxDoublePointListEntity();

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;

    Q_SIGNALS:
        ACE_TREE_DECLARE_RECORD_TABLE_SIGNALS(DspxDoublePointEntity)
    };
    //===========================================================================

    //===========================================================================
    // AnchorPoint
    class DspxAnchorPointEntityPrivate;

    class CORE_EXPORT DspxAnchorPointEntity : public DspxIntPointEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxAnchorPointEntity)
    public:
        explicit DspxAnchorPointEntity(QObject *parent = nullptr);
        ~DspxAnchorPointEntity();

    public:
        bool read(const QJsonValue &value) override;
        QJsonValue write() const override;

    public:
        enum Interpolation {
            None,
            Linear,
            Hermite,
        };
        Q_ENUM(Interpolation)

        Interpolation interp() const;
        void setInterp(Interpolation i);

    protected:
        void doInitialize() override;
        void doSetup() override;

    Q_SIGNALS:
        void interpolationChanged(Interpolation i);
    };
    //===========================================================================

    //===========================================================================
    // AnchorPoint List
    class CORE_EXPORT DspxAnchorPointListEntity : public AceTreeEntityRecordTable,
                                                  public AceTreeEntityRecordTableHelper<DspxAnchorPointEntity> {
        Q_OBJECT
    public:
        explicit DspxAnchorPointListEntity(QObject *parent = nullptr);
        ~DspxAnchorPointListEntity();

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;

    Q_SIGNALS:
        ACE_TREE_DECLARE_RECORD_TABLE_SIGNALS(DspxAnchorPointEntity)
    };
    //===========================================================================

}

#endif // CHORUSKIT_DSPXBASEENTITY_H
