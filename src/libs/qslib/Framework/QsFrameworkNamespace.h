#ifndef CHORUSKIT_QSFRAMEWORKNAMESPACE_H
#define CHORUSKIT_QSFRAMEWORKNAMESPACE_H

#include <QObject>

#include "QsFrameworkGlobal.h"

namespace QsApi {

    Q_NAMESPACE_EXPORT(QSFRAMEWORK_API)

    enum Role {
        DisplayRole = Qt::DisplayRole,
        DecorationRole = Qt::DecorationRole,

        // Customized
        SubtitleRole = Qt::UserRole + 2000,
        DescriptionRole,
        CategoryRole,
        EnumerationRole,
        SeparatorRole,
        IconSizeRole,
        ObjectPointerRole,
        AlignmentRole,
        InternalDataRole,
        InternalTypeRole,
        KeywordRole,

        UserRole = Qt::UserRole + 4000,
    };

    Q_ENUM_NS(Role)

}

#endif // CHORUSKIT_QSFRAMEWORKNAMESPACE_H
