#ifndef DSPXENTITY_P_H
#define DSPXENTITY_P_H

#include <QJsonObject>

#include <AceTreeStandardEntity_p.h>

#include "DspxRootEntity.h"

namespace Core {

    class DspxMetadataEntity : public AceTreeEntityMapping {
        Q_OBJECT
    public:
        explicit DspxMetadataEntity(QObject *parent = nullptr);
        ~DspxMetadataEntity();

        QString name() const override;

        friend class DspxRootEntity;
    };

    class DspxRootEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxRootEntityPrivate();
        ~DspxRootEntityPrivate();

        DspxMetadataEntity *metadata;
        DspxContentEntity *content;
        DspxWorkspaceEntity *workspace;
    };

    class DspxContentEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxContentEntityPrivate();
        ~DspxContentEntityPrivate();

        DspxMasterEntity *master;
        DspxTimelineEntity *timeline;
        DspxTrackListEntity *tracks;
        DspxWorkspaceEntity *workspace;
    };

    class DspxMasterEntityPrivate : public AceTreeStandardEntityPrivate {
    public:
        DspxMasterEntityPrivate();
        ~DspxMasterEntityPrivate();

        DspxBusControlEntity *control;
    };

}

#endif // DSPXENTITY_P_H
