#include "ImportDialog.h"
#include "private/ImportDialog_p.h"

#include "CCheckBox.h"

Q_D_LAYOUT_PROPERTY_DECLARE(buttons, Buttons, buttonsLayout, ImportDialog)
Q_D_LAYOUT_PROPERTY_DECLARE(boxes, Boxes, boxesLayout, ImportDialog)

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
    d->lbCaption->setText(options.caption);

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
    for (auto it = options.tracks.begin(); it != options.tracks.end(); ++it) {
        auto box = new CCheckBox(it->title);
        box->setObjectName("check-box");
        box->setEnabled(it->selectable);
        bg->addButton(box, i++);
        bl->addWidget(box);

        connect(box, &QAbstractButton::toggled, this, &ImportDialog::_q_boxToggled);
    }
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

void ImportDialog::_q_boxToggled(bool checked) {
    Q_D(ImportDialog);
    auto btn = qobject_cast<QAbstractButton *>(sender());
    if (checked) {
        // Add
        {
            auto it = d->queue.pushBack(btn);
            d->queueMap.insert(btn, it);
        }

        // Remove earliest
        if (d->queue.size() > d->opt.maxTracks) {
            auto it = d->queue.begin();
            auto btn2 = it.value();
            d->queueMap.remove(btn2);
            d->queue.erase(it);
            btn2->setChecked(false);
        }
    } else {
        auto it = d->queueMap.find(btn);
        if (it != d->queueMap.end()) {
            d->queue.erase(it.value());
            d->queueMap.erase(it);
        }
    }
}

void ImportDialog::_q_okButtonClicked() {
    Q_D(ImportDialog);

    auto bl = d->boxesLayout;
    for (int i = 0; i < bl->count(); ++i) {
        if (qobject_cast<QAbstractButton *>(bl->itemAt(i)->widget())->isChecked()) {
            d->trackIndexs.append(i);
        }
    }
    accept();
}

void ImportDialog::_q_cancelButtonClicked() {
    reject();
}
