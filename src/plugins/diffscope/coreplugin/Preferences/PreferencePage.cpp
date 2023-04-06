//
// Created by Crs_1 on 2023/4/6.
//

#include "PreferencePage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

using namespace Core;

PreferencePage::~PreferencePage() noexcept {
    //TODO
}

QString PreferencePage::id() const {
    return "ChorusKit:Preference";
}

QString PreferencePage::title() const {
    return tr("Preference");
}

void PreferencePage::reloadStrings() {
    if(m_filterEdit) m_filterEdit->setPlaceholderText(tr("Filter"));
}

QWidget *PreferencePage::widget() {
    if(!m_widget) {
        m_widget = new QWidget;
        auto mainLayout = new QHBoxLayout;
        auto selectAreaLayout = new QVBoxLayout;
        auto optionsAreaLayout = new QVBoxLayout;
        m_treeWidget = new QTreeWidget;
        m_treeWidget->setHeaderHidden(true);
        m_filterEdit = new QLineEdit;
        m_titleLabel = new QLabel;
        m_descriptionLabel = new QLabel;
        m_stackedWidget = new QStackedWidget;
        selectAreaLayout->addWidget(m_filterEdit);
        selectAreaLayout->addWidget(m_treeWidget);
        optionsAreaLayout->addWidget(m_titleLabel);
        optionsAreaLayout->addWidget(m_descriptionLabel);
        optionsAreaLayout->addWidget(m_stackedWidget);
        mainLayout->addLayout(selectAreaLayout);
        mainLayout->addLayout(optionsAreaLayout);
        m_widget->setLayout(mainLayout);
    }
    return m_widget;
}

void PreferencePage::load() {

}

void PreferencePage::accept() {

}

void PreferencePage::finish() {

}

void PreferencePage::setPage(IOptionsPage *page) {
    m_stackedWidget->setCurrentWidget(page->widget());
    m_titleLabel->setText(page->title());
    m_descriptionLabel->setText(page->description());
}