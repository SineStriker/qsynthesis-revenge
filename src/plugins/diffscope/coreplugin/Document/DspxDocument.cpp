#include "DspxDocument.h"
#include "DspxDocument_p.h"

#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

#include <QMDecoratorV2.h>

#include "DspxSpec.h"
#include "ICore.h"

namespace Core {

    static int m_untitledIndex = 0;

    DspxDocumentPrivate::DspxDocumentPrivate() {
        vstMode = false;
        model = nullptr;
    }

    DspxDocumentPrivate::~DspxDocumentPrivate() {
    }

    void DspxDocumentPrivate::init() {
        Q_Q(DspxDocument);

        model = new QsApi::AceTreeModel(q);
        ctl = DspxSpec::instance()->controllerBuilder()->buildRoot<DspxRootController>();
        if (!ctl) {
            ICore::fatalError(q->dialogParent(), DspxDocument::tr("Failed to create file model."));
        }

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

        auto item = new QsApi::AceTreeItem("root");
        if (!DspxSpec::instance()->serializer()->readObject(doc.object(), item)) {
            delete item;
            q->setErrorMessage(DspxDocument::tr("Error occurred while parsing file!"));
            return false;
        }
        model->setRootItem(item);
        ctl->setup(item);

        return true;
    }

    bool DspxDocumentPrivate::saveFile(QByteArray *data) const {
        Q_Q(const DspxDocument);

        QJsonObject obj;
        if (!DspxSpec::instance()->serializer()->writeObject(&obj, model->rootItem())) {
            q->setErrorMessage(DspxDocument::tr("Error occurred while saving file!"));
            return false;
        }
        *data = QJsonDocument(obj).toJson(QJsonDocument::Compact); // Disable indent to reduce size

        return true;
    }

    DspxDocument::DspxDocument(QObject *parent) : DspxDocument(*new DspxDocumentPrivate(), parent) {
    }

    DspxDocument::~DspxDocument() {
    }

    QsApi::AceTreeModel *DspxDocument::model() const {
        Q_D(const DspxDocument);
        return d->model;
    }

    DspxRootController *DspxDocument::rootCtl() const {
        Q_D(const DspxDocument);
        return d->ctl;
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

    bool DspxDocument::saveRawData(QByteArray *data) {
        Q_D(DspxDocument);
        return d->saveFile(data);
    }

    void DspxDocument::makeNew() {
        Q_D(DspxDocument);

        ++m_untitledIndex;

        auto item = new QsApi::AceTreeItem("root");
        DspxSpec::instance()->serializer()->createObject(item);

        d->model->setRootItem(item);
        d->ctl->setup(item);
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
