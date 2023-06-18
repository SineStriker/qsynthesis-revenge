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
    class DspxEnvParamInfoEntity;

    //===========================================================================
    // ParamCurve
    class DspxParamCurveEntityPrivate;

    class CORE_EXPORT DspxParamCurveEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxParamCurveEntity)
        Q_PROPERTY(int start READ start WRITE setStart NOTIFY startChanged)
    public:
        enum Type {
            Anchor,
            Free,
        };

        explicit DspxParamCurveEntity(Type type, QObject *parent = nullptr);
        ~DspxParamCurveEntity();

        Type curveType() const;

    public:
        int start() const;
        void setStart(int start);

    Q_SIGNALS:
        void startChanged(int start);

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

    public:
        bool read(const QJsonValue &value) override;
        QJsonValue write() const override;

    public:
        int size() const;
        QVector<qint16> mid(int index, int size) const;
        QVector<qint16> values() const;
        inline void append(const QVector<qint16> &values) {
            insert(size(), values);
        }
        void replace(int index, const QVector<qint16> &values);
        void insert(int index, const QVector<qint16> &values);
        void remove(int index, const QVector<qint16> &values);

    Q_SIGNALS:
        void replaced(int index, const QVector<qint16> &values);
        void inserted(int index, const QVector<qint16> &values);
        void removed(int index, int count);

    protected:
        void doInitialize() override;
        void doSetup() override;
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
    class DspxParamInfoEntityPrivate;

    class CORE_EXPORT DspxParamInfoEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxParamInfoEntity)
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
        DspxParamInfoEntity(DspxParamInfoEntityPrivate &d, QObject *parent = nullptr);
    };
    //===========================================================================

    //===========================================================================
    // EnvParamInfo
    class DspxEnvParamInfoEntityPrivate;

    class CORE_EXPORT DspxEnvParamInfoEntity : public DspxParamInfoEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxEnvParamInfoEntity)
    public:
        explicit DspxEnvParamInfoEntity(QObject *parent = nullptr);
        ~DspxEnvParamInfoEntity();

    public:
        DspxParamCurveListEntity *envelope() const;
    };
    //===========================================================================

    //===========================================================================
    // ParamSet
    class DspxParamSetEntityPrivate;

    class CORE_EXPORT DspxParamSetEntity : public AceTreeEntityMapping {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxParamSetEntity)
    public:
        explicit DspxParamSetEntity(QObject *parent = nullptr);
        ~DspxParamSetEntity();

    public:
        DspxParamInfoEntity *pitch() const;

    protected:
        DspxParamSetEntity(DspxParamSetEntityPrivate &d, QObject *parent = nullptr);
    };
    //===========================================================================

}



#endif // CHORUSKIT_DSPXPARAMENTITY_H
