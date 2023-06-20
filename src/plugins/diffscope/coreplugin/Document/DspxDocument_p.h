#ifndef DSPXDOCUMENT_P_H
#define DSPXDOCUMENT_P_H

#include "CoreApi/IDocument_p.h"
#include "Document/Entities/DspxRootEntity.h"
#include "DspxDocument.h"

#include <QMChronSet.h>
#include <QMDisplayString.h>

namespace Core {

    class DspxDocumentPrivate : public IDocumentPrivate {
        Q_DECLARE_PUBLIC(DspxDocument)
    public:
        DspxDocumentPrivate();
        virtual ~DspxDocumentPrivate();

        void init();

        AceTreeModel *model;
        AceTreeTransaction *tx;
        DspxContentEntity *content;

        bool opened;
        bool vstMode;

        QFile *binLog;
        QFile *txtLog;

        void changeToOpen();
        void unshiftToRecent();

        bool checkNotOpened() const;

        bool readFile(const QByteArray &data);
        bool saveFile(QByteArray *data) const;
    };

}

#endif // DSPXDOCUMENT_P_H
