#ifndef DOCUMENTSYSTEM_H
#define DOCUMENTSYSTEM_H

#include "DocumentSpec.h"
#include "IDocument.h"

namespace Core {

    class DocumentSystemPrivate;

    class CKAPPCORE_API DocumentSystem : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DocumentSystem)
    public:
        explicit DocumentSystem(QObject *parent = nullptr);
        ~DocumentSystem();

    public:
        bool addDocument(DocumentSpec *doc);
        bool removeDocument(DocumentSpec *doc);
        bool removeDocument(const QString &id);
        DocumentSpec *document(const QString &id) const;
        QList<DocumentSpec *> documents() const;
        QStringList documentIds() const;

        // recent files
        void addRecentFile(const QString &fileName);
        void clearRecentFiles();
        QStringList recentFiles() const;

        // recent dirs
        void addRecentDir(const QString &fileName);
        void clearRecentDirs();
        QStringList recentDirs() const;

    public:
        bool openFileBrowse(DocumentSpec *spec, const QString &path = {}, QWidget *parent = nullptr) const;

        bool openDirBrowse(DocumentSpec *spec, const QString &path = {}, QWidget *parent = nullptr) const;

        bool saveFileBrowse(IDocument *iDoc, const QString &path = {}, QWidget *parent = nullptr) const;

        QString getOpenFileName(QWidget *parent, const QString &filters, const QString &path = {},
                                QString *selectedFilter = nullptr) const;

        QStringList getOpenFileNames(QWidget *parent, const QString &filters, const QString &path = {},
                                     QString *selectedFilter = nullptr) const;

        QString getExistingDirectory(QWidget *parent, const QString &path = {}) const;

        QString getSaveFileName(QWidget *parent, const QString &path = {}, const QString &filter = QString(),
                                QString *selectedFilter = nullptr) const;

    public:
        enum FixMode { ResolveLinks, KeepLinks };

        static QString fixFileName(const QString &fileName, FixMode fixmode);

    signals:
        void recentFilesChanged();
        void recentDirsChanged();

    protected:
        DocumentSystem(DocumentSystemPrivate &d, QObject *parent = nullptr);

        QScopedPointer<DocumentSystemPrivate> d_ptr;
    };

}

#endif // DOCUMENTSYSTEM_H
