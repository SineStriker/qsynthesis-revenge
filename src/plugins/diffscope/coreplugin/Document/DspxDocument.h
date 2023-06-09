#ifndef DSPXDOCUMENT_H
#define DSPXDOCUMENT_H

#include <CoreApi/IDocument.h>
#include <ItemModel/AceTreeModel.h>

#include "DspxController.h"

namespace Core {

    class DspxDocumentPrivate;

    class CORE_EXPORT DspxDocument : public IDocument {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxDocument)
    public:
        explicit DspxDocument(QObject *parent = nullptr);
        ~DspxDocument();

        QsApi::AceTreeModel *model() const;

        DspxRootController *rootCtl() const;

    public:
        bool open(const QString &filename) override;
        bool save(const QString &filename) override;

        bool openRawData(const QByteArray &data);
        bool saveRawData(QByteArray *data);

        void makeNew();

        bool isVSTMode() const;
        void setVSTMode(bool on);

        ReloadBehavior reloadBehavior(ChangeTrigger state, ChangeType type) const override;
        bool reload(ReloadFlag flag, ChangeType type) override;

        QString defaultPath() const override;
        QString suggestedFileName() const override;
        bool isModified() const override;

        void close() override;

    protected:
        DspxDocument(DspxDocumentPrivate &d, QObject *parent = nullptr);

        QScopedPointer<DspxDocumentPrivate> d_ptr;
    };

}

#endif // DSPXDOCUMENT_H