#include "DspxDocument.h"
#include "DspxDocument_p.h"

#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QStandardPaths>
#include <QVersionNumber>

#include <QMDecoratorV2.h>

#include "DspxSpec.h"
#include "ICore.h"

namespace Core {

    static int m_untitledIndex = 0;

    DspxDocumentPrivate::DspxDocumentPrivate() {
        opened = false;
        vstMode = false;
        model = nullptr;
        content = nullptr;
        tx = nullptr;

        binLog = nullptr;
        txtLog = nullptr;
    }

    DspxDocumentPrivate::~DspxDocumentPrivate() {
        delete binLog;
        delete txtLog;
    }

    void DspxDocumentPrivate::init() {
        Q_Q(DspxDocument);

        model = new AceTreeModel(q);
        tx = new AceTreeTransaction(model, q);

        ICore::instance()->documentSystem()->addDocument(q, true);
    }

    void DspxDocumentPrivate::changeToOpen() {
        Q_Q(DspxDocument);

        opened = true;

        binLog = new QFile(q);
        binLog->setFileName(q->logDirectory() + "/atm.dat");

        txtLog = new QFile(q);
        txtLog->setFileName(q->logDirectory() + "/atx.log");

        if (!binLog->open(QIODevice::WriteOnly) || !txtLog->open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(q->dialogParent(), DspxDocument::tr("Log Error"),
                                 DspxDocument::tr("Failed to create log!"));
            return;
        }

        model->startLogging(binLog);
        tx->startLogging(txtLog);
    }

    void DspxDocumentPrivate::unshiftToRecent() {
        Q_Q(DspxDocument);
        ICore::instance()->documentSystem()->addRecentFile(q->filePath());
    }

    bool DspxDocumentPrivate::checkNotOpened() const {
        if (!opened)
            return true;
        errMsg = DspxDocument::tr("File is opened");
        return false;
    }

    bool DspxDocumentPrivate::readFile(const QByteArray &data) {
        Q_Q(DspxDocument);

        // Parse json
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
            errMsg = DspxDocument::tr("Invalid file format!");
            return false;
        }

        auto obj = doc.object();
        QJsonValue value;

        // Get version
        value = obj.value("version");
        if (!value.isString()) {
            errMsg = DspxDocument::tr("Missing version field!");
            return false;
        }

        // Check version
        QString versionString = value.toString();
        QVersionNumber version = QVersionNumber::fromString(versionString);
        if (version > DspxSpec::currentVersion()) {
            errMsg = DspxDocument::tr("The specified file version(%1) is too high.!").arg(versionString);
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
            errMsg = DspxDocument::tr("Missing content field!");
            return false;
        }

        auto cont = new DspxContentEntity();
        cont->initialize();

        if (!cont->read(value.toObject())) {
            delete cont;
            errMsg = DspxDocument::tr("Error occurred while parsing file!");
            return false;
        }

        this->content = cont;
        model->setRootItem(const_cast<AceTreeItem *>(cont->treeItem()));

        return true;
    }

    bool DspxDocumentPrivate::saveFile(QByteArray *data) const {
        Q_Q(const DspxDocument);

        auto obj = content->write().toObject();
        if (obj.isEmpty()) {
            errMsg = DspxDocument::tr("Error occurred while saving file!");
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

    bool DspxDocument::open(const QString &fileName) {
        Q_D(DspxDocument);

        if (!d->checkNotOpened()) {
            return false;
        }

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            setErrorMessage(tr("Failed to open file!"));
            return false;
        }

        QByteArray data = file.readAll();
        file.close();

        if (!d->readFile(data)) {
            return false;
        }

        setFilePath(fileName);
        d->changeToOpen();

        d->unshiftToRecent();

        return true;
    }

    bool DspxDocument::save(const QString &fileName) {
        Q_D(DspxDocument);

        QByteArray data;
        if (!d->saveFile(&data)) {
            return false;
        }

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            setErrorMessage(tr("Failed to create file!"));
            return false;
        }
        file.write(data);
        file.close();

        if (!d->preferredDisplayName.isEmpty()) {
            setPreferredDisplayName({});
        }
        setFilePath(fileName);

        d->unshiftToRecent();

        return true;
    }

    bool DspxDocument::openRawData(const QString &suggestFileName, const QByteArray &data) {
        Q_D(DspxDocument);

        if (!d->checkNotOpened()) {
            return false;
        }
        if (!d->readFile(data)) {
            return false;
        }

        QFileInfo info(suggestFileName);
        setFilePath(info.fileName());
        setPreferredDisplayName(info.baseName());
        d->changeToOpen();

        return true;
    }

    bool DspxDocument::saveRawData(QByteArray *data) const {
        Q_D(const DspxDocument);
        return d->saveFile(data);
    }

    void DspxDocument::makeNew() {
        Q_D(DspxDocument);

        if (d->opened) {
            return;
        }

        auto content = new DspxContentEntity();
        content->initialize();
        d->model->setRootItem(const_cast<AceTreeItem *>(content->treeItem()));
        d->content = content;

        QString baseName = QString("Untitled-%1").arg(QString::number(++m_untitledIndex));
        setFilePath(baseName + ".dspx");
        setPreferredDisplayName(baseName);
        d->changeToOpen();
    }

    bool DspxDocument::recover(const QString &filename) {
        Q_D(DspxDocument);

        if (!d->checkNotOpened()) {
            return false;
        }

        auto err = [this](){
            setErrorMessage(tr("Fail to read log"));
        };

        // Read binary log
        {
            QFile file(logDirectory() + "/atm.dat");
            if (!file.open(QIODevice::ReadOnly)) {
                err();
                return false;
            }

            QByteArray data = file.readAll();
            file.close();

            if (!d->model->recover(data)) {
                err();
                return false;
            }
        }

        // Read text log
        {
            QFile file(logDirectory() + "/atx.log");
            if (!file.open(QIODevice::ReadOnly)) {
                err();
                return false;
            }

            QByteArray data = file.readAll();
            file.close();

            if (!d->tx->recover(data)) {
                err();
                return false;
            }
        }

        auto content = new DspxContentEntity();
        content->setup(d->model->rootItem());
        d->content = content;

        QFileInfo info(filename);
        setFilePath(info.fileName());
        setPreferredDisplayName(info.baseName());
        d->changeToOpen();

        return true;
    }

    bool DspxDocument::isVSTMode() const {
        Q_D(const DspxDocument);
        return d->vstMode;
    }

    bool DspxDocument::isOpened() const {
        Q_D(const DspxDocument);
        return d->opened;
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
        return tr("Untitled Project.dspx");
    }

    bool DspxDocument::isModified() const {
        Q_D(const DspxDocument);
        if (!d->opened)
            return false;
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

    DspxDocument::DspxDocument(DspxDocumentPrivate &d, QObject *parent) : IDocument(d, "org.ChorusKit.dspx", parent) {
        d.init();
    }

}
