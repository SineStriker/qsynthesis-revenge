#include "ConfigurableDialog.h"
#include "private/ConfigurableDialog_p.h"

#include <QMDecoratorV2.h>

namespace QsApi {

    ConfigurableDialogPrivate::ConfigurableDialogPrivate() {
        widget = nullptr;
    }

    ConfigurableDialogPrivate::~ConfigurableDialogPrivate() {
    }

    void ConfigurableDialogPrivate::init() {
        Q_Q(ConfigurableDialog);

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

        connect(okButton, &QPushButton::clicked, q, &ConfigurableDialog::accept);
        connect(cancelButton, &QPushButton::clicked, q, &ConfigurableDialog::reject);
        connect(applyButton, &QPushButton::clicked, this, &ConfigurableDialogPrivate::apply_helper);

        qIDec->installLocale(this, _LOC(ConfigurableDialogPrivate, this));
    }

    void ConfigurableDialogPrivate::reloadStrings() {
        okButton->setText(tr("OK"));
        cancelButton->setText(tr("Cancel"));
        applyButton->setText(tr("Apply"));
    }

    void ConfigurableDialogPrivate::apply_helper() {
        Q_Q(ConfigurableDialog);
        q->apply();
        emit q->applied();
    }

    ConfigurableDialog::ConfigurableDialog(QWidget *parent)
        : ConfigurableDialog(*new ConfigurableDialogPrivate(), parent) {
    }

    ConfigurableDialog::~ConfigurableDialog() {
    }

    void ConfigurableDialog::done(int r) {
        Q_D(ConfigurableDialog);

        if (r == Accepted) {
            d->apply_helper();
        }

        finish();
        emit finished();

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
    }

    void ConfigurableDialog::finish() {
    }

    ConfigurableDialog::ConfigurableDialog(ConfigurableDialogPrivate &d, QWidget *parent) : QDialog(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
