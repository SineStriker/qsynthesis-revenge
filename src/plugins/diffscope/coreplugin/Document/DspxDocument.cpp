#include "DspxDocument.h"
#include "DspxDocument_p.h"

#include <QStandardPaths>

#include <Dspx/QDspxModel.h>
#include <QMDecoratorV2.h>

#include "ICore.h"

namespace Core {

    static int m_untitledIndex = 0;

    DspxDocumentPrivate::DspxDocumentPrivate() {
        hasWatch = false;
        isVST = false;
    }

    DspxDocumentPrivate::~DspxDocumentPrivate() {
    }

    void DspxDocumentPrivate::init() {
        Q_Q(DspxDocument);
        ICore::instance()->documentSystem()->addDocument(q, true);
    }

    void DspxDocumentPrivate::unshiftToRecent() {
        Q_Q(DspxDocument);
        ICore::instance()->documentSystem()->addRecentFile(q->filePath());
    }

    DspxDocument::DspxDocument(QObject *parent) : DspxDocument(*new DspxDocumentPrivate(), parent) {
    }

    DspxDocument::~DspxDocument() {
    }

    bool DspxDocument::addObserver(IDspxObserver *observer) {
        Q_D(DspxDocument);
        if (!observer) {
            qWarning() << "Core::DspxDocument::addObserver(): trying to add null observer";
            return false;
        }
        if (d->observers.contains(observer)) {
            qWarning() << "Core::DspxDocument::addObserver(): trying to add duplicated observer:" << observer;
            return false;
        }
        d->observers.append(observer);
        return true;
    }

    bool DspxDocument::removeObserver(IDspxObserver *observer) {
        Q_D(DspxDocument);
        auto it = d->observers.find(observer);
        if (it == d->observers.end()) {
            qWarning() << "Core::DspxDocument::removeObserver(): observer does not exist:" << observer;
            return false;
        }
        d->observers.erase(it);
        return true;
    }

    QsApi::AceTreeModel *DspxDocument::model() const {
        Q_D(const DspxDocument);
        return d->model;
    }

    bool DspxDocument::open(const QString &filename) {
        Q_D(DspxDocument);

        QDspxModel model;
        if (!model.load(filename)) {
            setErrorMessage(tr("Failed to open %1").arg(filename));
            return false;
        }

        for (auto observer : d->observers) {
            QString err;
            if (!observer->read(model, nullptr, &err)) {
                setErrorMessage(err);
                return false;
            }
        }

        setFilePath(filename);
        d->unshiftToRecent();

        return true;
    }

    bool DspxDocument::save(const QString &filename) {
        Q_D(DspxDocument);

        QDspxModel model;

        for (auto observer : d->observers) {
            QString err;
            if (!observer->write(&model, nullptr, &err)) {
                setErrorMessage(err);
                return false;
            }
        }

        if (!model.save(filename)) {
            setErrorMessage(tr("Failed to save %1").arg(filename));
            return false;
        }

        setFilePath(filename);
        d->unshiftToRecent();

        return true;
    }

    bool DspxDocument::isVST() const {
        Q_D(const DspxDocument);
        return d->isVST;
    }

    bool DspxDocument::openVST(const QByteArray &bytes) {
        Q_D(DspxDocument);

        d->isVST = true;

        return false;
    }

    QString DspxDocument::defaultPath() const {
        return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }

    QString DspxDocument::suggestedFileName() const {
        Q_D(const DspxDocument);
        if (d->untitledFileName.isEmpty())
            d->untitledFileName = QString("Untitled-%1.dspx").arg(QString::number(++m_untitledIndex));
        return d->untitledFileName;
    }

    bool DspxDocument::isModified() const {
        return false;
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
