#include "ConfigurableDialog.h"
#include "ConfigurableDialog_p.h"

#include <QMDecorator.h>

namespace Core {

    ConfigurableDialogPrivate::ConfigurableDialogPrivate() {
    }

    ConfigurableDialogPrivate::~ConfigurableDialogPrivate() {
    }

    void ConfigurableDialogPrivate::init() {
        Q_Q(ConfigurableDialog);

        q->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        q->setWindowFlag(Qt::WindowCloseButtonHint, true);

        widget = nullptr;

        okButton = new CTabButton();
        cancelButton = new CTabButton();
        applyButton = new CTabButton();

        buttonsLayout = new QMEqualBoxLayout(QBoxLayout::LeftToRight);
        buttonsLayout->setAlignment(Qt::AlignRight);
        buttonsLayout->addWidget(okButton);
        buttonsLayout->addWidget(cancelButton);
        buttonsLayout->addWidget(applyButton);

        mainLayout = new QVBoxLayout();
        mainLayout->addLayout(buttonsLayout);

        q->setLayout(mainLayout);

        connect(okButton, &QPushButton::clicked, q, &ConfigurableDialog::accept);
        connect(cancelButton, &QPushButton::clicked, q, &ConfigurableDialog::reject);
        connect(applyButton, &QPushButton::clicked, q, &ConfigurableDialog::apply);

        qIDec->installLocale(this, {{}}, _LOC(ConfigurableDialogPrivate, this));
    }

    void ConfigurableDialogPrivate::reloadStrings() {
        okButton->setText(tr("OK"));
        cancelButton->setText(tr("Cancel"));
        applyButton->setText(tr("Apply"));
    }

    ConfigurableDialog::ConfigurableDialog(QWidget *parent)
        : ConfigurableDialog(*new ConfigurableDialogPrivate(), parent) {
    }

    ConfigurableDialog::~ConfigurableDialog() {
    }

    void ConfigurableDialog::done(int r) {
        if (r == Accepted) {
            apply();
        }
        finish();
        QDialog::done(r);
    }

    QWidget *ConfigurableDialog::widget() const {
        Q_D(const ConfigurableDialog);
        return d->widget;
    }

    QWidget *ConfigurableDialog::takeWidget() {
        Q_D(ConfigurableDialog);
        if (!d->widget)
            return nullptr;

        d->widget = nullptr;
        return d->mainLayout->takeAt(0)->widget();
    }

    void ConfigurableDialog::setWidget(QWidget *w) {
        Q_D(ConfigurableDialog);

        delete takeWidget();

        d->widget = w;
        d->mainLayout->insertWidget(0, w);
    }

    void ConfigurableDialog::apply() {
        emit applied();
    }

    void ConfigurableDialog::finish() {
        emit finished();
    }

    ConfigurableDialog::ConfigurableDialog(ConfigurableDialogPrivate &d, QWidget *parent) : QDialog(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
