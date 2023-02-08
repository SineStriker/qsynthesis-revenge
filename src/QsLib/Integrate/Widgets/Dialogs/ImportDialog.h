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
    Q_DECLARE_PRIVATE(ImportDialog)
public:
    explicit ImportDialog(QWidget *parent = nullptr);
    ~ImportDialog();

    void reloadStrings();

public:
    struct TrackInfo {
        QString title;
        QString subtitle;
        bool selectable;
    };

    struct ImportOptions {
        QString caption;
        std::function<QString(QTextCodec *)> previewer;
        int maxTracks;
        QList<TrackInfo> tracks;
    };

    ImportOptions options() const;
    void setImportOptions(const ImportOptions &options);

    QList<int> selectResult() const;
    QTextCodec *codecResult() const;

protected:
    ImportDialog(ImportDialogPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<ImportDialogPrivate> d_ptr;

private:
    void _q_boxToggled(bool checked);
    void _q_okButtonClicked();
    void _q_cancelButtonClicked();
};

#endif // IMPORTDIALOG_H
