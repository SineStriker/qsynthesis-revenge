#include "IDocument.h"
#include "IDocument_p.h"

#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QSettings>

#include <QMSystem.h>
#include <QMWidgetsHost.h>

#include "DocumentSystem_p.h"
#include "ICoreBase.h"

namespace Core {

    LogDirectory::~LogDirectory() {
        if (!autoRemove) {
            if (!logDir.isNull())
                logDir->setAutoRemove(false);
        } else {
            QMFs::rmDir(userLogDir);
        }
    }

    QString LogDirectory::logDirectory() const {
        if (!QMFs::isDirExist(userLogDir)) {
            if (logDir.isNull())
                logDir.reset(
                    new QTemporaryDir(QString("%1/%2-XXXXXXXX").arg(DocumentSystemPrivate::logBaseDir(), prefix)));
            return logDir->path();
        }

        return userLogDir;
    }

    void LogDirectory::setLogDirectory(const QString &dir) {
        QFileInfo info(dir);
        if (!info.isDir() || !info.isWritable())
            return;

        logDir.reset();
        userLogDir = dir;
    }

    IDocumentPrivate::IDocumentPrivate() {
    }

    IDocumentPrivate::~IDocumentPrivate() {
    }

    void IDocumentPrivate::init() {
        Q_Q(IDocument);
        logDir.prefix = q->id();
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

    void IDocumentPrivate::updateLogDesc() {
        Q_Q(IDocument);

        IDocumentSettings settings(logDir.logDirectory());
        settings.setFileName(q->filePath());
        settings.setDocType(q->id());
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

    QString IDocument::logDirectory() const {
        Q_D(const IDocument);
        return d->logDir.logDirectory();
    }

    void IDocument::setLogDirectory(const QString &dir) {
        Q_D(IDocument);
        d->logDir.setLogDirectory(dir);
    }

    bool IDocument::autoRemoveLogDirectory() const {
        Q_D(const IDocument);
        return d->logDir.autoRemove;
    }

    void IDocument::setAutoRemoveLogDirectory(bool autoRemove) {
        Q_D(IDocument);
        d->logDir.autoRemove = autoRemove;
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
        d->updateLogDesc();

        emit filePathChanged(oldName, d->filePath);
        emit changed();
    }

    QString IDocument::plainDisplayName() const {
        Q_D(const IDocument);
        QString filename;
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

    IDocumentSettings::IDocumentSettings(const QString &dir) : d(new IDocumentSettingsData()) {
        setDir(dir);
    }

    IDocumentSettings::~IDocumentSettings() {
    }

    IDocumentSettings::IDocumentSettings(const IDocumentSettings &other) : d(new IDocumentSettingsData(*other.d)) {
    }

    IDocumentSettings::IDocumentSettings(IDocumentSettings &&other) noexcept : d(other.d) {
    }

    QString IDocumentSettings::dir() const {
        return d->dir;
    }

    void IDocumentSettings::setDir(const QString &dir) {
        if (!QMFs::isDirExist(dir)) {
            d->dir.clear();
            d->settings.clear();
        } else {
            d->dir = QFileInfo(dir).canonicalFilePath();
            d->settings.reset(new QSettings(IDocumentSettingsData::descFile(d->dir), QSettings::IniFormat));
        }
    }

    bool IDocumentSettings::remove() const {
        return QMFs::rmFile(IDocumentSettingsData::descFile(d->dir));
    }

    QString IDocumentSettings::fileName() const {
        return d->settings.isNull() ? QString() : d->settings->value("File/Path").toString();
    }

    void IDocumentSettings::setFileName(const QString &fileName) {
        if (!d->settings)
            return;
        d->settings->setValue("File/Path", fileName);
    }

    QString IDocumentSettings::docType() const {
        return d->settings.isNull() ? QString() : d->settings->value("File/Editor").toString();
    }

    void IDocumentSettings::setDocType(const QString &docType) {
        if (!d->settings)
            return;
        d->settings->setValue("File/Editor", docType);
    }

}
