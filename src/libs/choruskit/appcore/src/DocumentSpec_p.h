#ifndef DOCUMENTSPECPRIVATE_H
#define DOCUMENTSPECPRIVATE_H

#include <QIcon>

#include "DocumentSpec.h"

namespace Core {

    class DocumentSpecPrivate {
        Q_DECLARE_PUBLIC(DocumentSpec)
    public:
        DocumentSpecPrivate();
        virtual ~DocumentSpecPrivate();

        void init();

        DocumentSpec *q_ptr;

        QString id;
        QString displayName;
        QString description;
        QIcon icon;
    };

}

#endif // DOCUMENTSPECPRIVATE_H