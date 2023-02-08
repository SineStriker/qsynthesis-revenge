#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QLayout>
#include <QTextCodec>

#include "QPixelSize.h"
#include "QsFrameworkMacros.h"
#include "QsIntegrateGlobal.h"

class ImportDialogPrivate;

class QSINTEGRATE_API ImportDialog : public QDialog {
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
        QByteArray lyrics;
        bool selectable;
    };

    struct ImportOptions {
        int maxTracks;
        QString format; // Display as format.arg(title)
        QList<TrackInfo> tracks;

        ImportOptions() : format("%1"){};
    };

    ImportOptions options() const;
    void setImportOptions(const ImportOptions &options);

    QList<int> selectResult() const;
    QTextCodec *codecResult() const;

protected:
    ImportDialog(ImportDialogPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<ImportDialogPrivate> d_ptr;
};

#endif // IMPORTDIALOG_H
