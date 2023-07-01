#ifndef DSPXPARAMENTITY_H
#define DSPXPARAMENTITY_H

#include "AceTreeNumbersEntity.h"
#include "DspxBaseEntity.h"

namespace Core {

    class DspxParamCurveEntity;
    class DspxParamFreeEntity;
    class DspxParamAnchorEntity;
    class DspxParamFreeDataEntity;
    class DspxParamCurveListEntity;
    class DspxParamInfoEntity;
    class DspxEnvParamInfoEntity;

    //===========================================================================
    // ParamCurve
    class DspxParamCurveEntityExtra;

    class CORE_EXPORT DspxParamCurveEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_PROPERTY(int start READ start WRITE setStart NOTIFY startChanged)
    public:
        enum Type {
            Anchor,
            Free,
        };

        ~DspxParamCurveEntity();

        Type curveType() const;

    public:
        int start() const;
        void setStart(int start);

    Q_SIGNALS:
        void startChanged(int start);

    protected:
        DspxParamCurveEntity(DspxParamCurveEntityExtra *extra, QObject *parent = nullptr);
    };
    //===========================================================================

    //===========================================================================
    // ParamFree
    class CORE_EXPORT DspxParamFreeEntity : public DspxParamCurveEntity {
        Q_OBJECT
    public:
        explicit DspxParamFreeEntity(QObject *parent = nullptr);
        ~DspxParamFreeEntity();

    public:
        int step() const;

        DspxParamFreeDataEntity *values() const;
    };
    //===========================================================================

    //===========================================================================
    // ParamFreeData
    class CORE_EXPORT DspxParamFreeDataEntity : public AceTreeNumbersEntity, public AceTreeEntityNumbersHelper<qint16> {
        Q_OBJECT
    public:
        explicit DspxParamFreeDataEntity(QObject *parent = nullptr);
        ~DspxParamFreeDataEntity();

    Q_SIGNALS:
        ACE_TREE_DECLARE_NUMBERS_SIGNALS(qint16)
    };
    //===========================================================================

    //===========================================================================
    // ParamAnchor
    class DspxParamAnchorEntityPrivate;

    class CORE_EXPORT DspxParamAnchorEntity : public DspxParamCurveEntity {
        Q_OBJECT
    public:
        explicit DspxParamAnchorEntity(QObject *parent = nullptr);
        ~DspxParamAnchorEntity();

    public:
        DspxAnchorPointListEntity *nodes() const;
    };
    //===========================================================================

    //===========================================================================
    // ParamCurve List
    class CORE_EXPORT DspxParamCurveListEntity : public AceTreeEntityRecordTable,
                                                 public AceTreeEntityRecordTableHelper<DspxParamCurveEntity> {
        Q_OBJECT
    public:
        explicit DspxParamCurveListEntity(QObject *parent = nullptr);
        ~DspxParamCurveListEntity();

    public:
        void sortRecords(QVector<AceTreeEntity *> &records) const override;

    Q_SIGNALS:
        ACE_TREE_DECLARE_RECORD_TABLE_SIGNALS(DspxParamCurveEntity)
    };
    //===========================================================================

    //===========================================================================
    // ParamInfo
    class DspxParamInfoEntityExtra;

    class CORE_EXPORT DspxParamInfoEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_PROPERTY(QJsonArray original READ original WRITE setOriginal NOTIFY originalChanged)
    public:
        explicit DspxParamInfoEntity(QObject *parent = nullptr);
        ~DspxParamInfoEntity();

    public:
        QJsonArray original() const;
        void setOriginal(const QJsonArray &original);

        DspxParamCurveListEntity *edited() const;

    Q_SIGNALS:
        void originalChanged(const QJsonArray &org);

    protected:
        DspxParamInfoEntity(DspxParamInfoEntityExtra *extra, QObject *parent = nullptr);
    };
    //===========================================================================

    //===========================================================================
    // EnvParamInfo
    class DspxEnvParamInfoEntityPrivate;

    class CORE_EXPORT DspxEnvParamInfoEntity : public DspxParamInfoEntity {
        Q_OBJECT
    public:
        explicit DspxEnvParamInfoEntity(QObject *parent = nullptr);
        ~DspxEnvParamInfoEntity();

    public:
        DspxParamCurveListEntity *envelope() const;
    };
    //===========================================================================

    //===========================================================================
    // ParamSet
    class DspxParamSetEntityExtra;

    class CORE_EXPORT DspxParamSetEntity : public AceTreeEntityMapping {
        Q_OBJECT
    public:
        explicit DspxParamSetEntity(QObject *parent = nullptr);
        ~DspxParamSetEntity();

    public:
        DspxParamInfoEntity *pitch() const;

    protected:
        DspxParamSetEntity(DspxParamSetEntityExtra *extra, QObject *parent = nullptr);
    };
    //===========================================================================

}



#endif // DSPXPARAMENTITY_H
