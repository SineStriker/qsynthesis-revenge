#include "DspxDocument.h"
#include "DspxDocument_p.h"

#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QVersionNumber>

#include <QMDecoratorV2.h>

#include "DspxSpec.h"
#include "ICore.h"

namespace Core {

    static int m_untitledIndex = 0;

    DspxDocumentPrivate::DspxDocumentPrivate() {
        vstMode = false;
        model = nullptr;
        content = nullptr;
    }

    DspxDocumentPrivate::~DspxDocumentPrivate() {
    }

    void DspxDocumentPrivate::init() {
        Q_Q(DspxDocument);

        model = new AceTreeModel(q);
        tx = new AceTreeTransaction(model, q);

        ICore::instance()->documentSystem()->addDocument(q, true);
    }

    void DspxDocumentPrivate::unshiftToRecent() {
        Q_Q(DspxDocument);
        ICore::instance()->documentSystem()->addRecentFile(q->filePath());
    }

    bool DspxDocumentPrivate::readFile(const QByteArray &data) {
        Q_Q(DspxDocument);

        // Parse json
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
            q->setErrorMessage(DspxDocument::tr("Invalid file format!"));
            return false;
        }

        auto obj = doc.object();
        QJsonValue value;

        // Get version
        value = obj.value("version");
        if (!value.isString()) {
            q->setErrorMessage(DspxDocument::tr("Missing version field!"));
            return false;
        }

        // Check version
        QString versionString = value.toString();
        QVersionNumber version = QVersionNumber::fromString(versionString);
        if (version > DspxSpec::currentVersion()) {
            q->setErrorMessage(DspxDocument::tr("The specified file version(%1) is too high.!").arg(versionString));
            return false;
        }

        // Get workspace
        value = obj.value("workspace");
        // Check plugins
        if (value.isObject()) {
        }

        // Get content
        value = obj.value("content");
        if (!value.isObject()) {
            q->setErrorMessage(DspxDocument::tr("Missing content field!"));
            return false;
        }

        auto content = new DspxContentEntity();
        content->initialize();

        if (!content->read(value.toObject())) {
            delete content;
            q->setErrorMessage(DspxDocument::tr("Error occurred while parsing file!"));
            return false;
        }

        this->content = content;
        model->setRootItem(const_cast<AceTreeItem *>(content->treeItem()));

        return true;
    }

    bool DspxDocumentPrivate::saveFile(QByteArray *data) const {
        Q_Q(const DspxDocument);

        auto obj = content->write().toObject();
        if (obj.isEmpty()) {
            q->setErrorMessage(DspxDocument::tr("Error occurred while saving file!"));
            return false;
        }

        QJsonObject docObj;

        // Write version
        docObj.insert("version", DspxSpec::currentVersion().toString());

        // Write workspace
        docObj.insert("workspace", QJsonObject()); // Workspace

        // Write content
        docObj.insert("content", obj);

        *data = QJsonDocument(docObj).toJson(QJsonDocument::Compact); // Disable indent to reduce size
        return true;
    }

    DspxDocument::DspxDocument(QObject *parent) : DspxDocument(*new DspxDocumentPrivate(), parent) {
    }

    DspxDocument::~DspxDocument() {
    }

    AceTreeTransaction *DspxDocument::TX() const {
        Q_D(const DspxDocument);
        return d->tx;
    }

    DspxContentEntity *DspxDocument::project() const {
        Q_D(const DspxDocument);
        return d->content;
    }

    bool DspxDocument::open(const QString &filename) {
        Q_D(DspxDocument);

        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            setErrorMessage(tr("Failed to open file!"));
            return false;
        }

        QByteArray data = file.readAll();
        file.close();

        if (!d->readFile(data)) {
            return false;
        }

        setFilePath(filename);
        d->unshiftToRecent();

        return true;
    }

    bool DspxDocument::save(const QString &filename) {
        Q_D(DspxDocument);

        QByteArray data;
        if (!d->saveFile(&data)) {
            return false;
        }

        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            setErrorMessage(tr("Failed to create file!"));
            return false;
        }
        file.write(data);
        file.close();

        setFilePath(filename);
        d->unshiftToRecent();

        return true;
    }

    bool DspxDocument::openRawData(const QByteArray &data) {
        Q_D(DspxDocument);
        return d->readFile(data);
    }

    bool DspxDocument::saveRawData(QByteArray *data) const {
        Q_D(const DspxDocument);
        return d->saveFile(data);
    }

    void DspxDocument::makeNew() {
        Q_D(DspxDocument);

        ++m_untitledIndex;

        auto content = new DspxContentEntity();
        content->initialize();
        d->model->setRootItem(const_cast<AceTreeItem *>(content->treeItem()));
        d->content = content;
    }

    bool DspxDocument::recover(const QString &filename) {
        Q_UNUSED(filename)

        setFilePath(QFileInfo(filename).fileName());

        return false;
    }

    bool DspxDocument::isVSTMode() const {
        Q_D(const DspxDocument);
        return d->vstMode;
    }

    void DspxDocument::setVSTMode(bool on) {
        Q_D(DspxDocument);
        d->vstMode = on;
        emit changed();
    }

    QString DspxDocument::defaultPath() const {
        return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }

    QString DspxDocument::suggestedFileName() const {
        Q_D(const DspxDocument);
        if (d->untitledFileName.isEmpty())
            d->untitledFileName = QString("Untitled-%1.dspx").arg(QString::number(m_untitledIndex));
        return d->untitledFileName;
    }

    bool DspxDocument::isModified() const {
        return false;
    }

    void DspxDocument::close() {
        if (!QFileInfo(filePath()).isFile()) {
            ICore::instance()->documentSystem()->removeRecentFile(filePath());
        }
        IDocument::close();
    }

    IDocument::ReloadBehavior DspxDocument::reloadBehavior(IDocument::ChangeTrigger state,
                                                           IDocument::ChangeType type) const {
        if (type == TypeRemoved) {
            return BehaviorAsk;
        }
        return BehaviorSilent;
    }

    bool DspxDocument::reload(IDocument::ReloadFlag flag, IDocument::ChangeType type) {
        emit changed();
        return true;
    }

    DspxDocument::DspxDocument(DspxDocumentPrivate &d, QObject *parent)
        : IDocument("org.ChorusKit.dspx", parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
