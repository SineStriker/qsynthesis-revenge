#ifndef DSPXDOCUMENT_P_H
#define DSPXDOCUMENT_P_H

#include "CoreApi/IDocument_p.h"
#include "Document/Entities/DspxRootEntity.h"
#include "DspxDocument.h"

#include <QMChronSet.h>
#include <QMDisplayString.h>

namespace Core {

    class LogDirectory {
    public:
        explicit LogDirectory();

        ~LogDirectory();

        QString logDirectory() const;
        void setLogDirectory(const QString &dir);

        bool autoRemove;

    private:
        mutable QScopedPointer<QTemporaryDir> logDir;
        QString userLogDir;
    };

    class DspxDocumentPrivate : public IDocumentPrivate {
        Q_DECLARE_PUBLIC(DspxDocument)
    public:
        DspxDocumentPrivate(DspxDocument::Mode mode);
        virtual ~DspxDocumentPrivate();

        void init();

        DspxDocument::Mode mode;

        AceTreeModel *model;
        DspxHistory *history;
        DspxContentEntity *content;
        QSettings *settings;
        LogDirectory logDir;

        bool opened;

        void changeToOpen();
        void changeToSaved();
        void unshiftToRecent();

        bool checkNotOpened() const;

        bool readFile(const QByteArray &data);
        bool saveFile(QByteArray *data) const;
    };

    class DspxHistoryData {
    public:
        DspxHistoryData(DspxDocumentPrivate *dp);
        ~DspxHistoryData();

        DspxDocumentPrivate *dp;
    };

}

#endif // DSPXDOCUMENT_P_H
