#include "CentralTabWidget.h"

#include "CPushButton.h"

CentralTabWidget::CentralTabWidget(QWidget *parent) : QScrollableTabWidget(parent) {
}

CentralTabWidget::~CentralTabWidget() {
}

void CentralTabWidget::tabInserted(int index) {
    auto button = new CPushButton();
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button->setCheckable(true);
    button->setAutoCheck(false);
    button->setObjectName("close-button");
    setCloseButton(index, button);
}

void CentralTabWidget::tabSelected(int index, int orgIndex) {
    if (orgIndex >= 0) {
        closeButton(orgIndex)->setChecked(false);
    }
    // Following line doesn't work at the beginning
    closeButton(index)->setChecked(true);
}

void CentralTabWidget::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    auto button = closeButton(currentIndex());
    if (button && !button->isChecked()) {
        button->setChecked(true);
    }
}
