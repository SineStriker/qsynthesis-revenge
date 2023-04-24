#ifndef DSPXDOCUMENTPRIVATE_H
#define DSPXDOCUMENTPRIVATE_H

#include "DspxDocument.h"

namespace Core {

    class DspxDocumentPrivate {
        Q_DECLARE_PUBLIC(DspxDocument)
    public:
        DspxDocumentPrivate();
        virtual ~DspxDocumentPrivate();

        void init();

        DspxDocument *q_ptr;

        bool hasWatch;
        mutable QString untitledFileName;

        void unshiftToRecent();
    };

}

#endif // DSPXDOCUMENTPRIVATE_H