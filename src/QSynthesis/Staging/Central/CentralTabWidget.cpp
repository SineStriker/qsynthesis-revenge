#include "CentralTabWidget.h"

#include "CPushButton.h"
#include "CentralTab.h"

#include <QApplication>
#include <QHoverEvent>

CentralTabWidget::CentralTabWidget(QWidget *parent) : QScrollableTabWidget(parent) {
}

CentralTabWidget::~CentralTabWidget() {
}

void CentralTabWidget::invalidateHover() const {
    QHoverEvent *event = new QHoverEvent(QEvent::HoverLeave, QPointF(-1, -1), QPointF());
    QApplication::postEvent(tabBar(), event); // Send a fake hover event to tab bar
}

void CentralTabWidget::tabInserted(int index) {
    auto button = new CPushButton();
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button->setCheckable(true);
    button->setAutoCheck(false);
    button->setObjectName("close-button");
    setCloseButton(index, button);

    auto tab = qobject_cast<CentralTab *>(widget(index));
    if (tab) {
        connect(tab, &CentralTab::titleChanged, this, &CentralTabWidget::_q_tabTitleChanged);
        connect(tab, &CentralTab::iconChanged, this, &CentralTabWidget::_q_tabIconChanged);
        setTabIcon(index, tab->icon());
        setTabText(index, tab->title());
        if (index == currentIndex()) {
            emit currentTabTextChanged(tab->title());
        }
    }
}

void CentralTabWidget::tabSelected(int index, int orgIndex) {
    if (orgIndex >= 0) {
        closeButton(orgIndex)->setChecked(false);
    }
    // Following line doesn't work at the beginning
    closeButton(index)->setChecked(true);
    emit currentTabTextChanged(tabText(index));
}

void CentralTabWidget::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    auto button = closeButton(currentIndex());
    if (button && !button->isChecked()) {
        button->setChecked(true);
    }
}

void CentralTabWidget::_q_tabTitleChanged(const QString &title) {
    auto tab = qobject_cast<CentralTab *>(sender());
    if (tab) {
        setTabText(indexOf(tab), title);
    }
    if (tab == currentWidget()) {
        emit currentTabTextChanged(title);
    }
}

void CentralTabWidget::_q_tabIconChanged(const QIcon &icon) {
    auto tab = qobject_cast<CentralTab *>(sender());
    if (tab) {
        setTabIcon(indexOf(tab), icon);
    }
}
