#ifndef CHORUSKIT_QSFRAMEWORKNAMESPACE_H
#define CHORUSKIT_QSFRAMEWORKNAMESPACE_H

#include <QObject>

namespace QsApi {

    Q_NAMESPACE

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

        UserRole = Qt::UserRole + 4000,
    };

    Q_ENUM_NS(Role)

}

#endif // CHORUSKIT_QSFRAMEWORKNAMESPACE_H
