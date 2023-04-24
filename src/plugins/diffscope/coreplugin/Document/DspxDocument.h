#ifndef DSPXDOCUMENT_H
#define DSPXDOCUMENT_H

#include <QStandardItemModel>

#include <CoreApi/IDocument.h>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class DspxDocumentPrivate;

    class CORE_EXPORT DspxDocument : public IDocument {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxDocument)
    public:
        explicit DspxDocument(QObject *parent = nullptr);
        ~DspxDocument();

        bool open(const QString &filename) override;
        bool save(const QString &filename) override;

        QString defaultPath() const override;
        QString suggestedFileName() const override;
        bool isModified() const override;
        ReloadBehavior reloadBehavior(ChangeTrigger state, ChangeType type) const override;
        bool reload(QString *errorString, ReloadFlag flag, ChangeType type) override;

    protected:
        DspxDocument(DspxDocumentPrivate &d, QObject *parent = nullptr);

        QScopedPointer<DspxDocumentPrivate> d_ptr;
    };

}

#endif // DSPXDOCUMENT_H