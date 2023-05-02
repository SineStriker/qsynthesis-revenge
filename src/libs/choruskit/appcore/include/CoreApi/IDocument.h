#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include <QObject>
#include <QSharedData>

#include "DocumentSpec.h"

namespace Core {

    class IDocumentPrivate;

    class CKAPPCORE_API IDocument : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IDocument)
    public:
        explicit IDocument(const QString &id, QObject *parent = nullptr);
        ~IDocument();

        enum ChangeTrigger { TriggerInternal, TriggerExternal };

        enum ChangeType { TypeContents, TypePermissions, TypeRemoved };

        enum ReloadBehavior { BehaviorAsk, BehaviorSilent };

        enum ReloadFlag { FlagReload, FlagIgnore };

        QString id() const;
        DocumentSpec *spec() const;

        QString logDirectory() const;
        void setLogDirectory(const QString &dir); // Must be a writable directory

        bool autoRemoveLogDirectory() const;
        void setAutoRemoveLogDirectory(bool autoRemove);

    public:
        virtual bool open(const QString &filename) = 0;
        virtual bool save(const QString &filename) = 0;

        QString errorMessage() const;
        void setErrorMessage(const QString &message) const;

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

        virtual ReloadBehavior reloadBehavior(ChangeTrigger state, ChangeType type) const;

        virtual bool reload(QString *errorString, ReloadFlag flag, ChangeType type) = 0;

        virtual void raise();
        virtual void close();

        virtual QWidget *dialogParent() const;

    signals:
        void changed();
        void mimeTypeChanged();

        void aboutToReload();
        void reloadFinished(bool success);

        void raiseRequested();
        void closeRequested();

        void filePathChanged(const QString &oldName, const QString &newName);

    protected:
        IDocument(IDocumentPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<IDocumentPrivate> d_ptr;

        friend class DocumentSystem;
    };

    class IDocumentSettingsData;

    class CKAPPCORE_API IDocumentSettings {
    public:
        IDocumentSettings(const QString &dir = {});
        ~IDocumentSettings();

        IDocumentSettings(const IDocumentSettings &other);
        IDocumentSettings(IDocumentSettings &&other) noexcept;

    public:
        QString dir() const;
        void setDir(const QString &dir);

        bool remove() const;

        QString fileName() const;
        void setFileName(const QString &fileName);

        QString docType() const;
        void setDocType(const QString &docType);

    private:
        QSharedDataPointer<IDocumentSettingsData> d;
    };

}

#endif // IDOCUMENT_H