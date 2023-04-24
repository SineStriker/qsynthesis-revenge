#include "DspxDocument.h"
#include "DspxDocument_p.h"

#include <QStandardPaths>

#include <Dspx/QDspxModel.h>

#include "ICore.h"

namespace Core {

    static int m_untitledIndex = 0;

    DspxDocumentPrivate::DspxDocumentPrivate() {
        hasWatch = false;
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

    bool DspxDocument::open(const QString &filename) {
        Q_D(DspxDocument);

        QDspxModel model;
        if (!model.load(filename)) {
            setErrorMessage(tr("Failed to open %1").arg(filename));
            return false;
        }

        setFilePath(filename);
        d->unshiftToRecent();

        return true;
    }

    bool DspxDocument::save(const QString &filename) {
        Q_D(DspxDocument);

        QDspxModel model;
        if (!model.save(filename)) {
            setErrorMessage(tr("Failed to save %1").arg(filename));
            return false;
        }

        setFilePath(filename);
        d->unshiftToRecent();

        return true;
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
        return IDocument::reloadBehavior(state, type);
    }

    bool DspxDocument::reload(QString *errorString, IDocument::ReloadFlag flag, IDocument::ChangeType type) {
        emit changed();
        return true;
    }

    DspxDocument::DspxDocument(DspxDocumentPrivate &d, QObject *parent)
        : IDocument("org.ChorusKit.dspx", parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
