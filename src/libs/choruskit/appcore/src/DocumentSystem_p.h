#ifndef DOCUMENTSYSTEM_P_H
#define DOCUMENTSYSTEM_P_H

#include "DocumentSystem.h"

#include <QMChronMap.h>
#include <QMChronSet.h>

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

        void saveOpenFileSettings() const;

        QMChronMap<QString, DocumentSpec *> docSpecs;
        QHash<QString, QMChronSet<DocumentSpec *>> extensionsMap;
        QHash<QString, QString> preferredExtensionIdMap;

        QStringList m_recentFiles;
        QStringList m_recentDirs;

        QMChronMap<IDocument *, QJsonObject> docInfos;

        mutable QString openFileLastVisitDir;
        mutable QString openDirLastVisitDir;
        mutable QString saveFileLastVisitDir;
        mutable bool selectAllWhenRecover;

        mutable bool openFilesSaved;
        void postSaveOpenFilesTask();

    private:
        void _q_docInfoChanged();
        void _q_documentDestroyed();
    };

} // namespace Core

#endif // DOCUMENTSYSTEM_P_H
