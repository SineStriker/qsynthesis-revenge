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
    class DspxWorkspaceEntity;

    //===========================================================================
    // BusControl
    class CORE_EXPORT DspxBusControlEntity : public AceTreeEntityMapping {
        Q_OBJECT
    public:
        explicit DspxBusControlEntity(QObject *parent = nullptr);
        ~DspxBusControlEntity();

        QString name() const override;

    public:
        double gain() const;
        void setGain(double gain);

        bool mute() const;
        void setMute(bool mute);
    };
    //===========================================================================

    //===========================================================================
    // TrackControl
    class CORE_EXPORT DspxTrackControlEntity : public DspxBusControlEntity {
        Q_OBJECT
    public:
        explicit DspxTrackControlEntity(QObject *parent = nullptr);
        ~DspxTrackControlEntity();

    public:
        double pan() const;
        void setPan(double pan);

        bool solo() const;
        void setSolo(bool solo);
    };
    //===========================================================================

    //===========================================================================
    // IntPoint
    class CORE_EXPORT DspxIntPointEntity : public AceTreeEntityMapping {
        Q_OBJECT
    public:
        explicit DspxIntPointEntity(QObject *parent = nullptr);
        ~DspxIntPointEntity();

        QString name() const override;

    public:
        int x() const;
        void setX(int x);

        int y() const;
        void setY(int y);
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

        QString name() const override;

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;
    };
    //===========================================================================

    //===========================================================================
    // DoublePoint
    class CORE_EXPORT DspxDoublePointEntity : public AceTreeEntityMapping {
        Q_OBJECT
    public:
        explicit DspxDoublePointEntity(QObject *parent = nullptr);
        ~DspxDoublePointEntity();

        QString name() const override;

    public:
        double x() const;
        void setX(double x);

        double y() const;
        void setY(double y);
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

        QString name() const override;

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;
    };
    //===========================================================================

    //===========================================================================
    // AnchorPoint
    class CORE_EXPORT DspxAnchorPointEntity : public DspxIntPointListEntity {
        Q_OBJECT
    public:
        explicit DspxAnchorPointEntity(QObject *parent = nullptr);
        ~DspxAnchorPointEntity();

        enum Interpolation {
            None,
            Linear,
            Hermite,
        };
        Q_ENUM(Interpolation)

    public:
        Interpolation interpolation() const;
        void setInterpolation(Interpolation interpolation);
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

        QString name() const override;

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;
    };
    //===========================================================================

    //===========================================================================
    // Workspace
    class DspxWorkspaceEntity : public AceTreeObjectEntity {
        Q_OBJECT
    public:
        explicit DspxWorkspaceEntity(QObject *parent = nullptr);
        ~DspxWorkspaceEntity();

        QString name() const override;

        friend class DspxRootEntity;
    };
    //===========================================================================

}

#endif // CHORUSKIT_DSPXBASEENTITY_H
