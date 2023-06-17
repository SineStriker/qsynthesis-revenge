#ifndef DOCUMENTSPECPRIVATE_H
#define DOCUMENTSPECPRIVATE_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the ChorusKit API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QIcon>

#include "DocumentSpec.h"

namespace Core {

    class CKAPPCORE_API DocumentSpecPrivate {
        Q_DECLARE_PUBLIC(DocumentSpec)
    public:
        DocumentSpecPrivate();
        virtual ~DocumentSpecPrivate();

        void init();

        DocumentSpec *q_ptr;

        QString id;
        QMDisplayString displayName;
        QMDisplayString description;
        QIcon icon;
    };

}

#endif // DOCUMENTSPECPRIVATE_H