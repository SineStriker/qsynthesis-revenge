#ifndef DOCUMENTWATCHER_H
#define DOCUMENTWATCHER_H

#include "IDocument.h"

namespace Core {

    class PromptHandler;

    class DocumentWatcherPrivate;

    class CKAPPCORE_API DocumentWatcher : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DocumentWatcher)
    public:
        explicit DocumentWatcher(QObject *parent = nullptr);
        ~DocumentWatcher();

        enum ReloadSetting {
            AlwaysAsk = 0,
            ReloadUnmodified = 1,
            IgnoreAll = 2,
        };

        ReloadSetting reloadSetting() const;
        void setReloadSetting(ReloadSetting setting);

        PromptHandler *promptHandler() const;
        PromptHandler *takePromptHandler();
        void setPromptHandler(PromptHandler *handler);

    public:
        // file pool to monitor
        void addDocuments(const QList<IDocument *> &documents, bool addWatcher = true);
        void addDocument(IDocument *document, bool addWatcher = true);
        bool removeDocument(IDocument *document);
        bool hasDocument(IDocument *document) const;
        QList<IDocument *> modifiedDocuments() const;
        IDocument *searchDocument(const QString &filePath) const;

    public:
        void renamedFile(const QString &from, const QString &to);

        void expectFileChange(const QString &fileName);
        void unexpectFileChange(const QString &fileName);

        bool saveDocument(IDocument *document, const QString &fileName = QString(), bool *isReadOnly = nullptr);

        /* Used to notify e.g. the code model to update the given files. Does *not*
           lead to any editors to reload or any other editor manager actions. */
        void notifyFilesChangedInternally(const QStringList &files);

    public:
        enum FixMode { ResolveLinks, KeepLinks };

        static QString fixFileName(const QString &fileName, FixMode fixmode);

    protected:
        virtual QString getSaveAsFileName(const IDocument *document, const QString &path, QWidget *parent) const;

        virtual void documentAdded(IDocument *document, bool addWatch);
        virtual void documentChanged(IDocument *document);
        virtual void documentRemoved(IDocument *document);

    signals:
        /* Used to notify e.g. the code model to update the given files. Does *not*
           lead to any editors to reload or any other editor manager actions. */
        void filesChangedInternally(const QStringList &files);
        /// emitted if all documents changed their name e.g. due to the file changing on disk
        void allDocumentsRenamed(const QString &from, const QString &to);
        /// emitted if one document changed its name e.g. due to save as
        void documentRenamed(Core::IDocument *document, const QString &from, const QString &to);

    protected:
        DocumentWatcher(DocumentWatcherPrivate &d, QObject *parent = nullptr);

        QScopedPointer<DocumentWatcherPrivate> d_ptr;
    };

    class FileChangeBlockerPrivate;

    class CKAPPCORE_API FileChangeBlocker {
    public:
        FileChangeBlocker(const QString &fileName, DocumentWatcher *watcher);
        ~FileChangeBlocker();

    private:
        QScopedPointer<FileChangeBlockerPrivate> d;
    };

    class CKAPPCORE_API PromptHandler : public QObject {
        Q_OBJECT
    public:
        explicit PromptHandler(QObject *parent = nullptr);
        ~PromptHandler();

        enum ReloadPromptAnswer {
            ReloadCurrent,
            ReloadAll,
            ReloadSkipCurrent,
            ReloadNone,
            CloseCurrent,
        };

        virtual ReloadPromptAnswer reloadPrompt(const QString &fileName, bool modified, QWidget *parent);

        enum FileDeletedPromptAnswer {
            FileDeletedClose,
            FileDeletedCloseAll,
            FileDeletedSaveAs,
            FileDeletedSave,
        };

        virtual FileDeletedPromptAnswer fileDeletedPrompt(const QString &fileName, bool triggerExternally,
                                                          QWidget *parent);
    };

}

#endif // DOCUMENTWATCHER_H