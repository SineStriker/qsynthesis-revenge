#include "IDocument.h"
#include "IDocument_p.h"

#include <QDebug>
#include <QFileInfo>

#include "ICoreBase.h"

namespace Core {

    IDocumentPrivate::IDocumentPrivate() {
    }

    IDocumentPrivate::~IDocumentPrivate() {
    }

    void IDocumentPrivate::init() {
        getSpec();
    }

    bool IDocumentPrivate::getSpec() {
        spec = ICoreBase::instance()->documentSystem()->docType(id);
        if (!spec) {
            qWarning() << "IDocument: document is not registered to Document:" << id;
            return false;
        }

        return true;
    }

    IDocument::IDocument(const QString &id, QObject *parent) : IDocument(*new IDocumentPrivate(), id, parent) {
    }

    IDocument::~IDocument() {
    }

    QString IDocument::errorMessage() const {
        Q_D(const IDocument);
        return d->errMsg;
    }

    void IDocument::setErrorMessage(const QString &message) {
        Q_D(IDocument);
        d->errMsg = message;
    }

    QString IDocument::filePath() const {
        Q_D(const IDocument);
        return d->filePath;
    }

    void IDocument::setFilePath(const QString &path) {
        Q_D(IDocument);
        if (d->filePath == path)
            return;

        auto oldName = d->filePath;
        d->filePath = path;
        emit filePathChanged(oldName, d->filePath);
        emit changed();
    }

    QString IDocument::plainDisplayName() const {
        Q_D(const IDocument);
        return d->preferredDisplayName.isEmpty() ? QFileInfo(d->filePath).fileName() : d->preferredDisplayName;
    }

    QString IDocument::preferredDisplayName() const {
        Q_D(const IDocument);
        return d->preferredDisplayName;
    }

    void IDocument::setPreferredDisplayName(const QString &name) {
        Q_D(IDocument);
        if (name == d->preferredDisplayName)
            return;
        d->preferredDisplayName = name;
        emit changed();
    }

    QString IDocument::displayName() const {
        Q_D(const IDocument);
        return d->uniqueDisplayName.isEmpty() ? plainDisplayName() : d->uniqueDisplayName;
    }

    QString IDocument::uniqueDisplayName() const {
        Q_D(const IDocument);
        return d->uniqueDisplayName;
    }

    void IDocument::setUniqueDisplayName(const QString &name) {
        Q_D(IDocument);
        d->uniqueDisplayName = name;
    }

    bool IDocument::isFileReadOnly() const {
        Q_D(const IDocument);
        if (d->filePath.isEmpty())
            return false;
        return !QFileInfo(d->filePath).isWritable();
    }

    bool IDocument::isTemporary() const {
        Q_D(const IDocument);
        return d->temporary;
    }

    void IDocument::setTemporary(bool temporary) {
        Q_D(IDocument);
        d->temporary = temporary;
    }

    QString IDocument::defaultPath() const {
        return {};
    }

    QString IDocument::suggestedFileName() const {
        return {};
    }

    QString IDocument::mimeType() const {
        Q_D(const IDocument);
        return d->mimeType;
    }

    void IDocument::setMimeType(const QString &mimeType) {
        Q_D(IDocument);
        if (d->mimeType != mimeType) {
            d->mimeType = mimeType;
            emit mimeTypeChanged();
        }
    }

    bool IDocument::isSaveAsAllowed() const {
        return false;
    }

    IDocument::ReloadBehavior IDocument::reloadBehavior(IDocument::ChangeTrigger state,
                                                        IDocument::ChangeType type) const {
        switch (type) {
            case TypePermissions:
                return BehaviorSilent;
            case TypeContents:
                if (state == TriggerInternal && !isModified())
                    return BehaviorSilent;
                break;
            default:
                break;
        }
        return BehaviorAsk;
    }

    void IDocument::close() {
        emit closeRequested();
    }

    QWidget *IDocument::dialogParent() const {
        auto p = parent();
        while (p->parent()) {
            p = p->parent();
        }
        return p->isWidgetType() ? qobject_cast<QWidget *>(p) : nullptr;
    }

    IDocument::IDocument(IDocumentPrivate &d, const QString &id, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;

        d.init();
    }

}
