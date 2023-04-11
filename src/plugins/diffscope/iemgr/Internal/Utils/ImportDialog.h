#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QLayout>
#include <QTextCodec>

#include "IEMgrGlobal.h"
#include "QPixelSize.h"

namespace IEMgr {

    class ImportDialogPrivate;

    class ImportDialog : public QDialog {
        Q_OBJECT
        Q_LAYOUT_PROPERTY_DELCARE
        Q_D_LAYOUT_PROPERTY(buttons, Buttons)
        Q_D_LAYOUT_PROPERTY(boxes, Boxes)
        Q_D_PROPERTY(QPixelSize, maxInitHeight, MaxInitHeight)
        Q_DECLARE_PRIVATE(ImportDialog)
    public:
        explicit ImportDialog(QWidget *parent = nullptr);
        ~ImportDialog();

        void reloadStrings();

    public:
        struct TrackInfo {
            QByteArray title;
            QList<QByteArray> lyrics;
            QString format; // Display as format.arg(title)
            bool selectable;
            TrackInfo() : TrackInfo({}, {}){};
            TrackInfo(const QByteArray &title, const QList<QByteArray> &lyrics)
                : title(title), lyrics(lyrics), format("%1"), selectable(true){};
        };

        struct ImportOptions {
            int minTracks;
            int maxTracks;
            QList<TrackInfo> tracks;
            QList<QByteArray> labels;
            ImportOptions() : minTracks(0), maxTracks(1){};
        };

        ImportOptions options() const;
        void setImportOptions(const ImportOptions &options);

        QList<int> selectResult() const;
        QTextCodec *codecResult() const;

        void testUtf8();

    protected:
        ImportDialog(ImportDialogPrivate &d, QWidget *parent = nullptr);

        QScopedPointer<ImportDialogPrivate> d_ptr;
    };

}

#endif // IMPORTDIALOG_H
