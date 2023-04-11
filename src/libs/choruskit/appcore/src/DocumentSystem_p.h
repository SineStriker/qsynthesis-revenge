#ifndef DOCUMENTSYSTEMPRIVATE_H
#define DOCUMENTSYSTEMPRIVATE_H

#include "DocumentSystem.h"

#include <Collections/QMChronMap.h>
#include <Collections/QMChronSet.h>

namespace Core {

    class DocumentSystemPrivate {
        Q_DECLARE_PUBLIC(DocumentSystem)
    public:
        DocumentSystemPrivate();
        virtual ~DocumentSystemPrivate();

        void init();

        void readSettings();
        void saveSettings() const;

        DocumentSystem *q_ptr;

        QMChronMap<QString, DocumentSpec *> documents;

        QStringList m_recentFiles;
        QStringList m_recentDirs;

        QString openFileLastVisitDir;
        QString openDirLastVisitDir;
        QString saveFileLastVisitDir;
    };

}

#endif // DOCUMENTSYSTEMPRIVATE_H