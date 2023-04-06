//
// Created by Crs_1 on 2023/4/6.
//

#include "TestOptionsPage.h"
#include <QHBoxLayout>

using namespace Core;

QString TestOptionsPage::id() const {
    return "ChorusKit:Test";
}

QString TestOptionsPage::title() const {
    return tr("Test");
}

QString TestOptionsPage::description() const {
    return tr("This is a test.");
}

QWidget *TestOptionsPage::widget() {
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

void TestOptionsPage::reloadStrings() {
    m_label->setText(tr("Test2"));
}

void TestOptionsPage::load() {

}

void TestOptionsPage::accept() {
    qDebug(QString("Test accepted: %1").arg(m_lineEdit->text()).toStdString().c_str());
}

void TestOptionsPage::finish() {

}

bool TestOptionsPage::match(const QString &pattern) const {
    return false;
}

TestOptionsPage::~TestOptionsPage() noexcept {
    m_widget->deleteLater();
}