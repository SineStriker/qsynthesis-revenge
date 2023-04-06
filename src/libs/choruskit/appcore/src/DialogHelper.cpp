//
// Created by Crs_1 on 2023/4/3.
//
#include "DialogHelper.h"
#include "DialogHelper_p.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "QMDecorator.h"

using namespace Core;

DialogHelper::DialogHelper(QObject *parent): DialogHelper(new DialogHelperPrivate, parent) {}

static DialogHelper *m_instance = nullptr;

DialogHelper::DialogHelper(DialogHelperPrivate *d, QObject *parent): QObject(parent), d_ptr(d) {
    m_instance = this;
    d->q_ptr = this;

    qIDec->installLocale(this, {{}}, _LOC(DialogHelperPrivate, d));
}

QMap<QString, IDialogPage *> &DialogHelper::dialogPages() {
    return d_ptr->dialogPages;
}

DialogHelper::~DialogHelper() noexcept {
    Q_D(DialogHelper);
    m_instance = nullptr;
    for(auto &item: d->dialogPages) {
        item->deleteLater();
    }
}

void DialogHelper::addDialogPage(IDialogPage *dialogPage) {
    Q_D(DialogHelper);
    if(dialogPages().contains(dialogPage->id())) {
        //TODO warning dialog page already added
        return;
    }
    dialogPages().insert(dialogPage->id(), dialogPage);
    connect(d, &DialogHelperPrivate::reloadStrings, dialogPage, [=](){
        dialogPage->reloadStrings();
    });
}

void DialogHelper::removeDialogPage(const QString &id) {
    Q_D(DialogHelper);
    auto it = dialogPages().find(id);
    if(it == dialogPages().end()) {
        //TODO warning dialog not exist
        return;
    }
    disconnect(d, &DialogHelperPrivate::reloadStrings, it.value(), nullptr);
    it.value()->deleteLater();
    dialogPages().erase(it);
}
IDialogPage *DialogHelper::getDialogPage(const QString &id) {
    auto it = dialogPages().find(id);
    if(it == dialogPages().end()) {
        //TODO warning dialog not exist
        return nullptr;
    }
    return it.value();
}

void DialogHelper::setDialogOpenState(const QString &id, QDialog* dialog, bool state) {
    Q_D(DialogHelper);
    if(state) {
        d->openedDialogs.insert(id, dialog);
    } else {
        auto it = d->openedDialogs.find(id);
        if(it != d->openedDialogs.end()) {
            d->openedDialogs.erase(it);
        }
    }
}

bool DialogHelper::getDialogOpenState(const QString &id) {
    Q_D(DialogHelper);
    return d->openedDialogs.contains(id);
}

QDialog *DialogHelper::prepareDialog(IDialogPage *dialogPage, QWidget *parent, bool modal, int controls) {
    Q_D(DialogHelper);
    auto *dialog = new QDialog(parent);
    dialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    dialog->setWindowFlag(Qt::WindowCloseButtonHint, static_cast<bool>(controls & CloseButton));

    auto *okButton = new QPushButton(tr("OK"));
    auto *cancelButton = new QPushButton(tr("Cancel"));
    auto *applyButton = new QPushButton(tr("Apply"));

    auto *dialogControlButtonsLayout = new QHBoxLayout;
    if(controls & OkButton) dialogControlButtonsLayout->addWidget(okButton);
    if(controls & CancelButton) dialogControlButtonsLayout->addWidget(cancelButton);
    if(controls & ApplyButton) dialogControlButtonsLayout->addWidget(applyButton);
    dialogControlButtonsLayout->setAlignment(Qt::AlignRight);

    auto *dialogMainLayout = new QVBoxLayout;
    dialogPage->reloadStrings();
    dialogMainLayout->addWidget(dialogPage->widget());
    dialogMainLayout->addLayout(dialogControlButtonsLayout);

    dialog->setLayout(dialogMainLayout);
    dialog->setWindowTitle(dialogPage->title());

    connect(d, &DialogHelperPrivate::reloadStrings, dialog, [=](){
        okButton->setText(tr("OK"));
        okButton->setText(tr("Cancel"));
        okButton->setText(tr("Apply"));

        dialog->setWindowTitle(dialogPage->title());
    });

    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    connect(applyButton, &QPushButton::clicked, dialogPage, [=](){
        dialogPage->accept();
        emit dialogPage->accepted();
    });
    return dialog;
}

void DialogHelper::finalizeDialog(IDialogPage *dialogPage, QDialog *dialog) {
    Q_D(DialogHelper);
    disconnect(d, &DialogHelperPrivate::reloadStrings, dialog, nullptr);
}

int DialogHelper::showDialog(const QString &id, QWidget* parent, int controls) {
    Q_D(DialogHelper);
    auto *dialogPage = getDialogPage(id);
    if(!dialogPage) return NotOpened;
    if(getDialogOpenState(id)) {
        //TODO warning dialog already opened
        return NotOpened;
    }
    auto dialog = prepareDialog(dialogPage, parent, true, controls);
    setDialogOpenState(id, dialog, true);
    connect(dialog, &QDialog::finished, [=](){
        dialogPage->widget()->setParent(nullptr);
        finalizeDialog(dialogPage, dialog);
    });

    dialogPage->load();

    auto result = dialog->exec();
    if(result == QDialog::Accepted) {
        dialogPage->accept();
        emit dialogPage->accepted();
    }
    dialogPage->finish();
    emit dialogPage->done(result);
    setDialogOpenState(id, dialog, false);
    return result;
}

void DialogHelper::showModelessDialog(const QString &id, QWidget* parent, int controls) {
    Q_D(DialogHelper);
    auto *dialogPage = getDialogPage(id);
    if(!dialogPage) return;
    if(getDialogOpenState(id)) {
        //TODO warning dialog already opened
        return;
    }
    auto dialog = prepareDialog(dialogPage, parent, true, controls);
    setDialogOpenState(id, dialog, true);
    connect(dialog, &QDialog::finished, [=](int result){
        dialogPage->widget()->setParent(nullptr);
        finalizeDialog(dialogPage, dialog);
        if(result == QDialog::Accepted) {
            dialogPage->accept();
            emit dialogPage->accepted();
        }
        dialogPage->finish();
        emit dialogPage->done(result);
        setDialogOpenState(id, dialog, false);
    });
    dialogPage->load();
    dialog->show();
}

void DialogHelper::closeDialog(const QString &id, int result) {
    Q_D(DialogHelper);
    auto it = d->openedDialogs.find(id);
    if(it == d->openedDialogs.end()) {
        //TODO warning not opened
        return;
    }
    auto dialog = it.value();
    emit dialog->finished(result);
    dialog->close();
}
