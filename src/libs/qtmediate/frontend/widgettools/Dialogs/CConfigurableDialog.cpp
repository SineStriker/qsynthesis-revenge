#include "CConfigurableDialog.h"
#include "CConfigurableDialog_p.h"

#include <QMDecoratorV2.h>

CConfigurableDialogPrivate::CConfigurableDialogPrivate() {
    widget = nullptr;
}

CConfigurableDialogPrivate::~CConfigurableDialogPrivate() {
}

void CConfigurableDialogPrivate::init() {
    Q_Q(CConfigurableDialog);

    q->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    q->setWindowFlag(Qt::WindowCloseButtonHint, true);

    okButton = new CTabButton();
    cancelButton = new CTabButton();
    applyButton = new CTabButton();

    buttonsLayout = new QMEqualBoxLayout(QBoxLayout::LeftToRight);
    buttonsLayout->setAlignment(Qt::AlignRight);
    buttonsLayout->addWidgetE(okButton);
    buttonsLayout->addWidgetE(cancelButton);
    buttonsLayout->addWidgetE(applyButton);

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonsLayout);

    q->setLayout(mainLayout);

    connect(okButton, &QPushButton::clicked, q, &CConfigurableDialog::accept);
    connect(cancelButton, &QPushButton::clicked, q, &CConfigurableDialog::reject);
    connect(applyButton, &QPushButton::clicked, this, &CConfigurableDialogPrivate::apply_helper);

    qIDec->installLocale(this, _LOC(CConfigurableDialogPrivate, this));
}

void CConfigurableDialogPrivate::reloadStrings() {
    okButton->setText(tr("OK"));
    cancelButton->setText(tr("Cancel"));
    applyButton->setText(tr("Apply"));
}

void CConfigurableDialogPrivate::apply_helper() {
    Q_Q(CConfigurableDialog);
    q->apply();
    emit q->applied();
}

CConfigurableDialog::CConfigurableDialog(QWidget *parent) : CConfigurableDialog(*new CConfigurableDialogPrivate(), parent) {
}

CConfigurableDialog::~CConfigurableDialog() {
}

void CConfigurableDialog::done(int r) {
    Q_D(CConfigurableDialog);

    if (r == Accepted) {
        d->apply_helper();
    }

    finish();
    emit finished();

    QDialog::done(r);
}

QWidget *CConfigurableDialog::widget() const {
    Q_D(const CConfigurableDialog);
    return d->widget;
}

QWidget *CConfigurableDialog::takeWidget() {
    Q_D(CConfigurableDialog);
    if (!d->widget)
        return nullptr;

    d->widget = nullptr;
    return d->mainLayout->takeAt(0)->widget();
}

void CConfigurableDialog::setWidget(QWidget *w) {
    Q_D(CConfigurableDialog);

    delete takeWidget();

    d->widget = w;
    d->mainLayout->insertWidget(0, w);
}

bool CConfigurableDialog::applyButtonVisible() const {
    Q_D(const CConfigurableDialog);
    return d->applyButton->isVisible();
}

void CConfigurableDialog::setApplyButtonVisible(bool visible) {
    Q_D(CConfigurableDialog);
    d->applyButton->setVisible(visible);
}

void CConfigurableDialog::apply() {
}

void CConfigurableDialog::finish() {
}

CConfigurableDialog::CConfigurableDialog(CConfigurableDialogPrivate &d, QWidget *parent) : QDialog(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
