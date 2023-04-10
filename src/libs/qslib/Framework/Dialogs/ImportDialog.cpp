#include "ImportDialog.h"
#include "private/ImportDialog_p.h"

#include <QCheckBox>
#include <QPushButton>
#include <QScrollBar>

#include "Widgets/CCheckBox.h"

namespace QsApi {

    Q_D_LAYOUT_PROPERTY_DECLARE(buttons, Buttons, buttonsLayout, ImportDialog)
    Q_D_LAYOUT_PROPERTY_DECLARE(boxes, Boxes, boxesLayout, ImportDialog)
    Q_D_PROPERTY_DECLARE(QPixelSize, maxInitHeight, MaxInitHeight, ImportDialog)

    ImportDialog::ImportDialog(QWidget *parent) : ImportDialog(*new ImportDialogPrivate(), parent) {
    }

    ImportDialog::~ImportDialog() {
    }

    void ImportDialog::reloadStrings() {
        Q_D(ImportDialog);
        d->btnOK->setText(tr("OK"));
        d->btnCancel->setText(tr("Cancel"));
    }

    ImportDialog::ImportOptions ImportDialog::options() const {
        Q_D(const ImportDialog);
        return d->opt;
    }

    void ImportDialog::setImportOptions(const ImportDialog::ImportOptions &options) {
        Q_D(ImportDialog);
        d->opt = options;

        auto bg = d->boxGroup;
        auto bl = d->boxesLayout;

        // Remove old buttons
        const auto &boxes = bg->buttons();
        for (const auto &box : boxes) {
            bl->removeWidget(box);
            bg->removeButton(box);
            box->deleteLater();
        }
        d->queue.clear();
        d->queueMap.clear();

        // Add new buttons
        int i = 0;
        bool noTitle = true;
        bool noLyrics = true;
        for (const auto &track : options.tracks) {
            auto box = new CCheckBox();
            box->setObjectName("check-box");
            box->setEnabled(track.selectable);
            bg->addButton(box, i++);
            bl->addWidget(box);
            if (!track.title.isEmpty()) {
                noTitle = false;
            }
            if (!track.lyrics.isEmpty()) {
                noLyrics = false;
            }
            connect(box, &QAbstractButton::toggled, d, &ImportDialogPrivate::_q_boxToggled);
        }

        bool noLabel = options.labels.isEmpty();

        d->labelsWidget->setVisible(!noLabel);
        d->nameListWidget->setVisible(!(noTitle && noLyrics));
        d->lyricsWidget->setVisible(!noLyrics);
        d->setCodecTabVisible(!(noTitle && noLyrics && noLabel));

        d->updateNameList();
    }

    QList<int> ImportDialog::selectResult() const {
        Q_D(const ImportDialog);
        return d->trackIndexs;
    }

    QTextCodec *ImportDialog::codecResult() const {
        Q_D(const ImportDialog);
        return d->codec;
    }

    ImportDialog::ImportDialog(ImportDialogPrivate &d, QWidget *parent) : QDialog(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}