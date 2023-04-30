#ifndef DOCUMENTSYSTEMPRIVATE_H
#define DOCUMENTSYSTEMPRIVATE_H

#include "DocumentSystem.h"

#include <Collections/QMChronMap.h>
#include <Collections/QMChronSet.h>

#include <QMSystem.h>

#include "DocumentWatcher_p.h"

namespace Core {

    class DocumentSystemPrivate : public DocumentWatcherPrivate {
        Q_OBJECT
        Q_DECLARE_PUBLIC(DocumentSystem)
    public:
        DocumentSystemPrivate();
        ~DocumentSystemPrivate();

        void init();

        void readSettings();
        void saveSettings() const;

        QMChronMap<QString, DocumentSpec *> docSpecs;
        QHash<QString, QMChronSet<DocumentSpec *>> extensionsMap;
        QHash<QString, QString> preferredExtensionIdMap;

        QStringList m_recentFiles;
        QStringList m_recentDirs;

        mutable QString openFileLastVisitDir;
        mutable QString openDirLastVisitDir;
        mutable QString saveFileLastVisitDir;
        mutable bool selectAllWhenRecover;

        static QString logBaseDir();
    };

}

#endif // DOCUMENTSYSTEMPRIVATE_H