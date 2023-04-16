#include "DspxDocument.h"
#include "DspxDocument_p.h"

#include "ICore.h"

namespace Core {

    static int m_untitledIndex = 0;

    DspxDocumentPrivate::DspxDocumentPrivate() {
    }

    DspxDocumentPrivate::~DspxDocumentPrivate() {
    }

    void DspxDocumentPrivate::init() {
    }

    DspxDocument::DspxDocument(QObject *parent) : DspxDocument(*new DspxDocumentPrivate(), parent) {
    }

    DspxDocument::~DspxDocument() {
    }

    bool DspxDocument::open(const QString &filename) {
        return false;
    }

    bool DspxDocument::save(const QString &filename) {
        return false;
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
        return false;
    }

    DspxDocument::DspxDocument(DspxDocumentPrivate &d, QObject *parent)
        : IDocument("org.ChorusKit.dspx", parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
