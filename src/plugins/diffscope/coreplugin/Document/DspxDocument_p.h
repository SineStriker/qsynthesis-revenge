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

        mutable QString untitledFileName;
    };

}

#endif // DSPXDOCUMENTPRIVATE_H