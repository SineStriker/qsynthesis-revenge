#ifndef CHORUSKIT_QSFRAMEWORKNAMESPACE_H
#define CHORUSKIT_QSFRAMEWORKNAMESPACE_H

#include <QObject>

namespace QsApi {

    Q_NAMESPACE

    enum Role {
        SubtitleRole = Qt::UserRole + 2000,
        DescriptionRole,
        CategoryRole,
        IconSizeRole,
    };

    Q_ENUM_NS(Role)

}

#endif // CHORUSKIT_QSFRAMEWORKNAMESPACE_H
