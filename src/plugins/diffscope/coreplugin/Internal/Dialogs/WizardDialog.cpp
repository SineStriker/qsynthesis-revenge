#include "WizardDialog.h"

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

    void WizardDialog::refreshPage() {
        auto label = new QLabel(QString(20, QString::number(step).front()));
        label->setStyleSheet("QLabel { border: 2px solid red; }");

        setWidget(label);
        setPrevEnabled(step > 0);
        setNextEnabled(step < 5);
    }
}