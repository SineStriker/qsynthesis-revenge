#include "WizardDialog.h"

#include <QDebug>
#include <QLabel>

#include <QMDecorator.h>

#include "ICore.h"

namespace Core::Internal {

    WizardDialog::WizardDialog(QWidget *parent) : WorkflowDialog(parent) {
        step = 0;

        refreshPage();

        qIDec->installLocale(this, {{}}, _LOC(WizardDialog, this));
    }

    WizardDialog::~WizardDialog() {
    }

    void WizardDialog::reloadStrings() {
    }

    void WizardDialog::prev() {
        step--;
        refreshPage();
    }

    void WizardDialog::next() {
        step++;
        refreshPage();
    }

    void WizardDialog::finish() {
        qDebug() << "Wizard finished";
    }

    void WizardDialog::refreshPage() {
        auto label = new QLabel(QString(20, QString::number(step).front()));
        label->setStyleSheet("QLabel { border: 2px solid red; }");

        auto page = new WorkflowPage();
        page->setTitle("My Title " + QString::number(step));
        page->setDescription("My Description " + QString::number(step));
        page->setWidget(label);

        page->setButtons(step < 5 ? WorkflowPage::StandardButtons : WorkflowPage::FinalButtons);
        page->setButtonEnabled(WorkflowPage::PreviousButton, step > 0);

        setPage(page);
    }
}