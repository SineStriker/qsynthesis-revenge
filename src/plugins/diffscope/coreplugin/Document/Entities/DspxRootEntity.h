#ifndef CHORUSKIT_DSPXROOTENTITY_H
#define CHORUSKIT_DSPXROOTENTITY_H

#include "DspxBaseEntity.h"
#include "DspxTimelineEntity.h"
#include "DspxTrackEntity.h"

namespace Core {

    class DspxRootEntity;
    class DspxContentEntity;
    class DspxMasterEntity;

    //===========================================================================
    // Root
    class DspxRootEntityPrivate;

    class CORE_EXPORT DspxRootEntity : public AceTreeStandardEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxRootEntity)
    public:
        explicit DspxRootEntity(QObject *parent = nullptr);
        ~DspxRootEntity();

        QString name() const override;

    public:
        QString version() const;
        void setVersion(const QString &version);

        QString author() const;
        void setAuthor(const QString &author);

        QString projectName() const;
        void setProjectName(const QString &projectName);

        DspxWorkspaceEntity *workspace() const;
        DspxContentEntity *content() const;
    };
    //===========================================================================

    //===========================================================================
    // Content
    class DspxContentEntityPrivate;

    class CORE_EXPORT DspxContentEntity : public AceTreeStandardEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxContentEntity)
    public:
        explicit DspxContentEntity(QObject *parent = nullptr);
        ~DspxContentEntity();

        QString name() const override;

    public:
        DspxMasterEntity *master() const;
        DspxTimelineEntity *timeline() const;
        DspxTrackListEntity *tracks() const;
        DspxWorkspaceEntity *workspace() const;
    };
    //===========================================================================

    //===========================================================================
    // Master
    class DspxMasterEntityPrivate;

    class CORE_EXPORT DspxMasterEntity : public AceTreeStandardEntity {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxMasterEntity)
    public:
        explicit DspxMasterEntity(QObject *parent = nullptr);
        ~DspxMasterEntity();

        QString name() const override;

    public:
        DspxBusControlEntity *control() const;
    };
    //===========================================================================

}

#endif // CHORUSKIT_DSPXROOTENTITY_H
