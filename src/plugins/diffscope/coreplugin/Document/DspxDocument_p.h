#ifndef DSPXDOCUMENTPRIVATE_H
#define DSPXDOCUMENTPRIVATE_H

#include "DspxController.h"
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

        mutable QString untitledFileName;

        QsApi::AceTreeModel *model;
        DspxRootController *ctl;

        bool vstMode;

        void unshiftToRecent();

        bool readFile(const QByteArray &data);
        bool saveFile(QByteArray *data) const;
    };

}

#endif // DSPXDOCUMENTPRIVATE_H