#include "IDocument.h"
#include "IDocument_p.h"

#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QSettings>

#include <QMSystem.h>

#include "DocumentSystem_p.h"
#include "ICoreBase.h"

namespace Core {

    IDocumentPrivate::IDocumentPrivate() {
    }

    IDocumentPrivate::~IDocumentPrivate() {
    }

    void IDocumentPrivate::init() {
        Q_Q(IDocument);
        getSpec();
    }

    bool IDocumentPrivate::getSpec() {
        spec = ICoreBase::instance()->documentSystem()->docType(id);
        if (!spec) {
            qWarning() << "Core::IDocument(): document is not registered to Document:" << id;
            return false;
        }
        return true;
    }

    IDocument::IDocument(const QString &id, QObject *parent) : IDocument(*new IDocumentPrivate(), id, parent) {
    }

    IDocument::~IDocument() {
    }

    QString IDocument::id() const {
        Q_D(const IDocument);
        return d->id;
    }

    DocumentSpec *IDocument::spec() const {
        Q_D(const IDocument);
        return d->spec;
    }

    QJsonObject IDocument::docInfo() const {
        Q_D(const IDocument);
        return d->docInfo;
    }

    void IDocument::setDocInfo(const QJsonObject &docInfo) {
        Q_D(IDocument);
        d->docInfo = docInfo;
        emit docInfoChanged();
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

    QString IDocument::errorMessage() const {
        Q_D(const IDocument);
        return d->errMsg;
    }

    void IDocument::setErrorMessage(const QString &message) const {
        Q_D(const IDocument);
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

        // Update descriptions in log file
        // d->updateLogDesc();

        emit filePathChanged(oldName, d->filePath);
        emit changed();
    }

    QString IDocument::plainDisplayName() const {
        Q_D(const IDocument);
        QString filename;
        if (!d->preferredDisplayName.isEmpty())
            return d->preferredDisplayName;
        return d->preferredDisplayName.isEmpty() ? ((filename = QFileInfo(d->filePath).fileName()).isEmpty()
                                                        ? QFileInfo(suggestedFileName()).baseName()
                                                        : filename)
                                                 : d->preferredDisplayName;
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
        QFileInfo file(d->filePath);
        if (file.isRelative() || !file.isFile())
            return false;
        return !file.isWritable();
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

    bool IDocument::isSaveAsAllowed() const {
        return true;
    }

    void IDocument::raise() {
        emit raiseRequested();
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

} // namespace Core
