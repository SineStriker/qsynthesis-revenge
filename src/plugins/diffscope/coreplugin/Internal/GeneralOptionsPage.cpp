//
// Created by Crs_1 on 2023/4/6.
//

#include "GeneralOptionsPage.h"
#include <QHBoxLayout>

using namespace Core;

QString GeneralOptionsPage::id() const {
    return "ChorusKit:General";
}

QString GeneralOptionsPage::title() const {
    return tr("General");
}

QString GeneralOptionsPage::description() const {
    return tr("General options.");
}

QWidget *GeneralOptionsPage::widget() {
    if(!m_widget) {
        m_widget = new QWidget;
        auto mainLayout = new QVBoxLayout;
        m_label = new QLabel;
        m_lineEdit = new QLineEdit;
        mainLayout->addWidget(m_label);
        mainLayout->addWidget(m_lineEdit);
        m_widget->setLayout(mainLayout);
    }
    return m_widget;
}

void GeneralOptionsPage::reloadStrings() {
    m_label->setText(tr("Test"));
}

void GeneralOptionsPage::load() {

}

void GeneralOptionsPage::accept() {

}

void GeneralOptionsPage::finish() {

}

bool GeneralOptionsPage::match(const QString &pattern) const {
    return false;
}

GeneralOptionsPage::~GeneralOptionsPage() noexcept {
    m_widget->deleteLater();
}