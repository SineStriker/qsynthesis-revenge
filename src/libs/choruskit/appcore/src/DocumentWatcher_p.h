#ifndef DOCUMENTWATCHERPRIVATE_H
#define DOCUMENTWATCHERPRIVATE_H

#include <QDateTime>
#include <QFile>
#include <QFileSystemWatcher>
#include <QMap>
#include <QSet>

#include "DocumentWatcher.h"

namespace Core {

    struct FileStateItem {
        QDateTime modified;
        QFile::Permissions permissions;
    };

    struct FileState {
        QMap<IDocument *, FileStateItem> lastUpdatedState;
        FileStateItem expected;
    };

    class DocumentWatcherPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(DocumentWatcher)
    public:
        DocumentWatcherPrivate();
        ~DocumentWatcherPrivate();

        void init();

        QFileSystemWatcher *fileWatcher();
        QFileSystemWatcher *linkWatcher();

        void addFileInfo(const QString &fileName, IDocument *document, bool isLink);
        void addFileInfo(IDocument *document);
        void removeFileInfo(IDocument *document);
        void updateExpectedState(const QString &fileName);

        DocumentWatcher *q_ptr;

        DocumentWatcher::ReloadSetting reloadSetting;
        PromptHandler *promptHandler;

        // File monitor valuables
        QMap<QString, FileState> m_states;
        QSet<QString> m_changedFiles;
        QList<IDocument *> m_documentsWithoutWatch;
        QMap<IDocument *, QStringList> m_documentsWithWatch;
        QSet<QString> m_expectedFileNames;

        QFileSystemWatcher *m_fileWatcher; // Delayed creation.
        QFileSystemWatcher *m_linkWatcher; // Delayed creation (only UNIX/if a link is seen).
        bool m_blockActivated;

        // When we are calling into an IDocument
        // we don't want to receive a changed()
        // signal
        // That makes the code easier
        IDocument *m_blockedIDocument;

        bool saveDocument_helper(IDocument *document, const QString &fileName, bool *isReadOnly);

        static void errorOnOverwrite(const QString &fileName, QWidget *parent);

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;

    private slots:
        void documentDestroyed(QObject *obj);
        void checkForNewFileName();
        void checkForReload();
        void changedFile(const QString &file);

        void filePathChanged(const QString &oldName, const QString &newName);
    };

}

#endif // DOCUMENTWATCHERPRIVATE_H
