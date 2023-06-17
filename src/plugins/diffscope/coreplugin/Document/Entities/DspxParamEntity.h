//
// Created by Functioner on 2023/6/17.
//

#ifndef CHORUSKIT_DSPXPARAMENTITY_H
#define CHORUSKIT_DSPXPARAMENTITY_H

#include "DspxBaseEntity.h"

namespace Core {

    class DspxParamCurveEntity;
    class DspxParamFreeEntity;
    class DspxParamAnchorEntity;
    class DspxParamFreeDataEntity;
    class DspxParamCurveListEntity;
    class DspxParamInfoEntity;

    //===========================================================================
    // ParamCurve
    class DspxParamCurveEntityPrivate;

    class CORE_EXPORT DspxParamCurveEntity : public AceTreeStandardEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxParamCurveEntity)
    public:
        enum Type {
            Anchor,
            Free,
        };

        explicit DspxParamCurveEntity(Type type, AceTreeStandardEntity::Type schemaType, QObject *parent = nullptr);
        ~DspxParamCurveEntity();

        Type curveType() const;

    public:
        int start() const;
        void setStart(int start);

    protected:
        DspxParamCurveEntity(DspxParamCurveEntityPrivate &d, QObject *parent = nullptr);
    };
    //===========================================================================

    //===========================================================================
    // ParamFree
    class DspxParamFreeEntityPrivate;

    class CORE_EXPORT DspxParamFreeEntity : public DspxParamCurveEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxParamFreeEntity)
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
    class DspxParamFreeDataEntityPrivate;

    class CORE_EXPORT DspxParamFreeDataEntity : public AceTreeEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxParamFreeDataEntity)
    public:
        explicit DspxParamFreeDataEntity(QObject *parent = nullptr);
        ~DspxParamFreeDataEntity();

        QString name() const override;

    public:
        bool read(const QJsonValue &value) override;
        QJsonValue write() const override;
    };

    //===========================================================================

    //===========================================================================
    // ParamAnchor
    class DspxParamAnchorEntityPrivate;

    class CORE_EXPORT DspxParamAnchorEntity : public DspxParamCurveEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxParamAnchorEntity)
    public:
        explicit DspxParamAnchorEntity(QObject *parent = nullptr);
        ~DspxParamAnchorEntity();

    public:
        DspxIntPointListEntity *nodes() const;
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
    };
    //===========================================================================

    //===========================================================================
    // ParamInfo
    class DspxParamInfoEntityPrivate;

    class CORE_EXPORT DspxParamInfoEntity : public AceTreeStandardEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxParamInfoEntity)
    public:
        explicit DspxParamInfoEntity(QObject *parent = nullptr);
        ~DspxParamInfoEntity();

    public:
        QJsonArray original() const;
        void setOriginal(const QJsonArray &original);

        DspxParamCurveListEntity *edited() const;

        DspxParamCurveListEntity *envelope() const;
    };
    //===========================================================================

    //===========================================================================
    // ParamSet
    class CORE_EXPORT DspxParamSetEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxParamAnchorEntity)
    public:
        explicit DspxParamSetEntity(QObject *parent = nullptr);
        ~DspxParamSetEntity();
    };
    //===========================================================================

}



#endif // CHORUSKIT_DSPXPARAMENTITY_H
