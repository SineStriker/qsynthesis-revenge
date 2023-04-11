#ifndef IDOCUMENTPRIVATE_H
#define IDOCUMENTPRIVATE_H

#include "IDocument.h"
#include "DocumentSpec.h"

namespace Core {

    class IDocumentPrivate {
        Q_DECLARE_PUBLIC(IDocument)
    public:
        IDocumentPrivate();
        virtual ~IDocumentPrivate();

        void init();

        bool getSpec();

        IDocument *q_ptr;

        QString id;
        DocumentSpec *spec;

        QString errMsg;

        QString mimeType;
        QString filePath;
        QString preferredDisplayName;
        QString uniqueDisplayName;
        QString autoSaveName;
        bool temporary;
    };

}

#endif // IDOCUMENTPRIVATE_H