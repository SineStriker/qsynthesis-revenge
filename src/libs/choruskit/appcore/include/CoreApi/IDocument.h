#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include <QObject>

#include "CkAppCoreGlobal.h"

namespace Core {

    class IDocumentPrivate;

    class CKAPPCORE_API IDocument : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IDocument)
    public:
        explicit IDocument(const QString &id, QObject *parent = nullptr);
        ~IDocument();

    public:
        virtual bool open(const QString &filename) = 0;
        virtual bool save(const QString &filename) = 0;

        QString errorMessage() const;
        void setErrorMessage(const QString &message);

        QString filePath() const;
        virtual void setFilePath(const QString &path);

        QString plainDisplayName() const;
        QString preferredDisplayName() const;
        void setPreferredDisplayName(const QString &name);
        QString displayName() const;
        QString uniqueDisplayName() const;
        void setUniqueDisplayName(const QString &name);

        virtual bool isFileReadOnly() const;
        bool isTemporary() const;
        void setTemporary(bool temporary);

        virtual QString defaultPath() const;
        virtual QString suggestedFileName() const;

        QString mimeType() const;
        void setMimeType(const QString &mimeType);

        virtual bool isModified() const = 0;
        virtual bool isSaveAsAllowed() const;

    signals:
        void changed();
        void mimeTypeChanged();

        void filePathChanged(const QString &oldName, const QString &newName);

    protected:
        IDocument(IDocumentPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<IDocumentPrivate> d_ptr;

        friend class DocumentSystem;
    };

}

#endif // IDOCUMENT_H