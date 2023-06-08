#ifndef DSPXDOCUMENTPRIVATE_H
#define DSPXDOCUMENTPRIVATE_H

#include "DspxDocument.h"

#include <Collections/QMChronSet.h>

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

        QsApi::AceTreeModel *model;

        bool vstMode;

        void unshiftToRecent();

        bool readFile(const QByteArray &data);
        bool saveFile(QByteArray *data) const;
    };

}

#endif // DSPXDOCUMENTPRIVATE_H