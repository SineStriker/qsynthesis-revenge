#ifndef DOCUMENTSYSTEM_H
#define DOCUMENTSYSTEM_H

#include "DocumentSpec.h"
#include "DocumentWatcher.h"

namespace Core {

    class DocumentSystemPrivate;

    class CKAPPCORE_API DocumentSystem : public DocumentWatcher {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DocumentSystem)
    public:
        explicit DocumentSystem(QObject *parent = nullptr);
        ~DocumentSystem();

        static QString logBaseDir();

    public:
        bool addDocType(DocumentSpec *doc);
        bool removeDocType(DocumentSpec *doc);
        bool removeDocType(const QString &id);
        DocumentSpec *docType(const QString &id) const;
        QList<DocumentSpec *> docTypes() const;
        QStringList docTypeIds() const;

        DocumentSpec *supportedDocType(const QString &suffix) const;
        QList<DocumentSpec *> supportedDocTypes(const QString &suffix) const;
        QStringList supportedExtensions() const;

        QString preferredDocTypeId(const QString &suffix) const;
        void setPreferredDocTypeId(const QString &suffix, const QString &id);

        // recent files
        void addRecentFile(const QString &fileName);
        void removeRecentFile(const QString &fileName);
        void clearRecentFiles();
        QStringList recentFiles() const;

        // recent dirs
        void addRecentDir(const QString &fileName);
        void clearRecentDirs();
        QStringList recentDirs() const;

        bool openFileBrowse(QWidget *parent, DocumentSpec *spec, const QString &path = {}) const;

        bool saveFileBrowse(QWidget *parent, IDocument *doc, const QString &path = {}) const;

    public:
        QString getOpenFileName(QWidget *parent, const QString &title, const QString &filters, const QString &path = {},
                                QString *selectedFilter = nullptr) const;

        QStringList getOpenFileNames(QWidget *parent, const QString &title, const QString &filters,
                                     const QString &path = {}, QString *selectedFilter = nullptr) const;

        QString getExistingDirectory(QWidget *parent, const QString &title, const QString &path = {}) const;

        QString getSaveFileName(QWidget *parent, const QString &title, const QString &path = {},
                                const QString &filter = QString(), QString *selectedFilter = nullptr) const;

    protected:
        QString getSaveAsFileName(const IDocument *document, const QString &path, QWidget *parent) const override;

        void documentAdded(IDocument *document, bool addWatch) override;
        void documentChanged(IDocument *document) override;
        void documentRemoved(IDocument *document) override;

    signals:
        void recentFilesChanged();
        void recentDirsChanged();

    protected:
        DocumentSystem(DocumentSystemPrivate &d, QObject *parent = nullptr);
    };

}

#endif // DOCUMENTSYSTEM_H
