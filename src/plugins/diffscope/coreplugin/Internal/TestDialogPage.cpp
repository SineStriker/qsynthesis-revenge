//
// Created by Crs_1 on 2023/4/4.
//

#include "TestDialogPage.h"
#include <QHBoxLayout>

using namespace Core;

TestDialogPage::~TestDialogPage() noexcept {
    delete m_widget;
}

QString TestDialogPage::id() const {
    return "test";
}

QString TestDialogPage::title() const {
    return "Test 114514";
}

QWidget *TestDialogPage::widget() {
    if(m_widget) {
        return m_widget;
    } else {
        m_widget = new QWidget;
        m_label = new QLabel("114514");
        m_lineEdit = new QLineEdit("1919810");
        auto layout = new QHBoxLayout;
        layout->addWidget(m_label);
        layout->addWidget(m_lineEdit);
        m_widget->setLayout(layout);
        return m_widget;
    }
}

void TestDialogPage::load() {

}

void TestDialogPage::accept() {

}

void TestDialogPage::finish() {

}

QString TestDialogPage::getEditText() const  {
    if(m_lineEdit) return m_lineEdit->text();
    else return "";
}

void TestDialogPage::setLabelText(const QString &text)  {
    if(m_label) m_label->setText(text);
};