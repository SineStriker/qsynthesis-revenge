#ifndef DSPXDOCUMENT_P_H
#define DSPXDOCUMENT_P_H

#include "DspxDocument.h"

#include <QMChronSet.h>

namespace Core {

    class DspxDocumentPrivate {
        Q_DECLARE_PUBLIC(DspxDocument)
    public:
        DspxDocumentPrivate();
        virtual ~DspxDocumentPrivate();

        void init();

        DspxDocument *q_ptr;

        mutable QString untitledFileName;

        AceTreeModel *model;

        bool vstMode;

        void unshiftToRecent();

        bool readFile(const QByteArray &data);
        bool saveFile(QByteArray *data) const;
    };

}

#endif // DSPXDOCUMENT_P_H
