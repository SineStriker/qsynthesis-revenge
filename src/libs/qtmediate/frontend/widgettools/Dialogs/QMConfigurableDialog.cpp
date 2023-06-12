#include "QMConfigurableDialog.h"
#include "QMConfigurableDialog_p.h"

#include <QMDecoratorV2.h>

QMConfigurableDialogPrivate::QMConfigurableDialogPrivate() {
    widget = nullptr;
}

QMConfigurableDialogPrivate::~QMConfigurableDialogPrivate() {
}

void QMConfigurableDialogPrivate::init() {
    Q_Q(QMConfigurableDialog);

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

    connect(okButton, &QPushButton::clicked, q, &QMConfigurableDialog::accept);
    connect(cancelButton, &QPushButton::clicked, q, &QMConfigurableDialog::reject);
    connect(applyButton, &QPushButton::clicked, this, &QMConfigurableDialogPrivate::apply_helper);

    qIDec->installLocale(this, _LOC(QMConfigurableDialogPrivate, this));
}

void QMConfigurableDialogPrivate::reloadStrings() {
    okButton->setText(tr("OK"));
    cancelButton->setText(tr("Cancel"));
    applyButton->setText(tr("Apply"));
}

void QMConfigurableDialogPrivate::apply_helper() {
    Q_Q(QMConfigurableDialog);
    q->apply();
    emit q->applied();
}

QMConfigurableDialog::QMConfigurableDialog(QWidget *parent) : QMConfigurableDialog(*new QMConfigurableDialogPrivate(), parent) {
}

QMConfigurableDialog::~QMConfigurableDialog() {
}

void QMConfigurableDialog::done(int r) {
    Q_D(QMConfigurableDialog);

    if (r == Accepted) {
        d->apply_helper();
    }

    finish();
    emit finished();

    QDialog::done(r);
}

QWidget *QMConfigurableDialog::widget() const {
    Q_D(const QMConfigurableDialog);
    return d->widget;
}

QWidget *QMConfigurableDialog::takeWidget() {
    Q_D(QMConfigurableDialog);
    if (!d->widget)
        return nullptr;

    d->widget = nullptr;
    return d->mainLayout->takeAt(0)->widget();
}

void QMConfigurableDialog::setWidget(QWidget *w) {
    Q_D(QMConfigurableDialog);

    delete takeWidget();

    d->widget = w;
    d->mainLayout->insertWidget(0, w);
}

bool QMConfigurableDialog::applyButtonVisible() const {
    Q_D(const QMConfigurableDialog);
    return d->applyButton->isVisible();
}

void QMConfigurableDialog::setApplyButtonVisible(bool visible) {
    Q_D(QMConfigurableDialog);
    d->applyButton->setVisible(visible);
}

void QMConfigurableDialog::apply() {
}

void QMConfigurableDialog::finish() {
}

QMConfigurableDialog::QMConfigurableDialog(QMConfigurableDialogPrivate &d, QWidget *parent) : QDialog(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
