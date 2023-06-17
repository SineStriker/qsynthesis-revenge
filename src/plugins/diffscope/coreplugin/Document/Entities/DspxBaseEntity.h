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

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;
    };
    //===========================================================================

    //===========================================================================
    // AnchorPoint
    class CORE_EXPORT DspxAnchorPointEntity : public DspxIntPointEntity {
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

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;
    };
    //===========================================================================

}

#endif // CHORUSKIT_DSPXBASEENTITY_H
