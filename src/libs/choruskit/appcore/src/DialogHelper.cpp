//
// Created by Crs_1 on 2023/4/3.
//
#include "DialogHelper.h"
#include "DialogHelper_p.h"
#include <QVBoxLayout>
#include <QPushButton>

using namespace Core;

DialogHelper::DialogHelper(QObject *parent): DialogHelper(new DialogHelperPrivate, parent) {}

static DialogHelper *m_instance = nullptr;

DialogHelper::DialogHelper(DialogHelperPrivate *d, QObject *parent): QObject(parent), d_ptr(d) {
    m_instance = this;
    d->q_ptr = this;
}

QMap<QString, IDialogPage *> &DialogHelper::dialogs() {
    return d_ptr->dialogs;
}

DialogHelper::~DialogHelper() noexcept {
    Q_D(DialogHelper);
    m_instance = nullptr;
    for(auto &item: d->dialogs) {
        delete item;
    }
}

void DialogHelper::addDialogPage(IDialogPage *dialogPage) {

    dialogs().insert(dialogPage->id(), dialogPage);
}

void DialogHelper::removeDialogPage(const QString &id) {
    auto it = dialogs().find(id);
    if(it == dialogs().end()) {
        //TODO warning dialog not exist
        return;
    }
    dialogs().erase(it);
}
IDialogPage *DialogHelper::getDialogPage(const QString &id) {
    auto it = dialogs().find(id);
    if(it == dialogs().end()) {
        return nullptr;
    }
    return it.value();
}

void DialogHelper::setDialogOpenState(IDialogPage *dialogPage, bool state) {
    Q_D(DialogHelper);
    if(state) {
        d->openedDialogPages.insert(dialogPage);
    } else {
        auto it = d->openedDialogPages.find(dialogPage);
        if(it != d->openedDialogPages.end()) {
            d->openedDialogPages.erase(it);
        }
    }
}

bool DialogHelper::getDialogOpenState(IDialogPage *dialogPage) {
    Q_D(DialogHelper);
    return d->openedDialogPages.contains(dialogPage);
}

QDialog *DialogHelper::prepareDialog(IDialogPage *dialogPage, QWidget *parent, bool modal, int controls) {
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
    dialogMainLayout->addWidget(dialogPage->widget());
    dialogMainLayout->addLayout(dialogControlButtonsLayout);

    dialog->setLayout(dialogMainLayout);
    dialog->setWindowTitle(dialogPage->title());
    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    connect(applyButton, &QPushButton::clicked, dialogPage, [=](){
        dialogPage->accept();
        emit dialogPage->accepted();
    });
    return dialog;
}

int DialogHelper::showDialog(const QString &id, QWidget* parent, int controls) {
    Q_D(DialogHelper);
    auto it = dialogs().find(id);
    if(it == dialogs().end()) {
        //TODO warning dialog not exist
        return NotOpened;
    }
    auto *dialogPage = it.value();
    if(getDialogOpenState(dialogPage)) {
        //TODO warning dialog already opened
        return NotOpened;
    }
    setDialogOpenState(dialogPage, true);
    auto dialog = prepareDialog(dialogPage, parent, true, controls);
    connect(dialog, &QDialog::finished, [=](){
        dialogPage->widget()->setParent(nullptr);
    });

    dialogPage->load();

    auto result = dialog->exec();
    if(result == QDialog::Accepted) {
        dialogPage->accept();
        emit dialogPage->accepted();
    }
    dialogPage->finish();
    emit dialogPage->done(result);
    setDialogOpenState(dialogPage, false);
    return result;
}

void DialogHelper::showModelessDialog(const QString &id, QWidget* parent, int controls) {
    Q_D(DialogHelper);
    auto it = dialogs().find(id);
    if(it == dialogs().end()) {
        //TODO warning dialog not exist
        return;
    }
    auto *dialogPage = it.value();
    if(getDialogOpenState(dialogPage)) {
        //TODO warning dialog already opened
        return;
    }
    setDialogOpenState(dialogPage, true);
    auto dialog = prepareDialog(dialogPage, parent, true, controls);
    connect(dialog, &QDialog::finished, [=](int result){
        dialogPage->widget()->setParent(nullptr);
        if(result == QDialog::Accepted) {
            dialogPage->accept();
            emit dialogPage->accepted();
        }
        dialogPage->finish();
        emit dialogPage->done(result);
        setDialogOpenState(dialogPage, false);
    });
    dialogPage->load();
    dialog->show();
}
