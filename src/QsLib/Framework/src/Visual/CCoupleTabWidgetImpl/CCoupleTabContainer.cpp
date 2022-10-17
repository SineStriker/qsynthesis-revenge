#include "CCoupleTabContainer.h"

CCoupleTabContainer::CCoupleTabContainer(QWidget *parent) : QSplitter(parent) {
    init();
}

CCoupleTabContainer::CCoupleTabContainer(Qt::Orientation orient, QWidget *parent)
    : QSplitter(orient, parent) {
    init();
}

CCoupleTabContainer::~CCoupleTabContainer() {
}

void CCoupleTabContainer::init() {
    setChildrenCollapsible(false);

    m_firstWidget = new QStackedWidget();
    m_secondWidget = new QStackedWidget();

    QSplitter::addWidget(m_firstWidget);
    QSplitter::addWidget(m_secondWidget);

    m_firstWidget->hide();
    m_secondWidget->hide();
    updateVisible();
}

int CCoupleTabContainer::addWidget(CCoupleTabTypes::Number num, QWidget *w, bool visible) {
    return insertWidget(
        num, (num == CCoupleTabTypes::First) ? m_firstWidget->count() : m_secondWidget->count(), w,
        visible);
}

int CCoupleTabContainer::insertWidget(CCoupleTabTypes::Number num, int index, QWidget *w,
                                      bool visible) {
    QStackedWidget *container = (num == CCoupleTabTypes::First) ? m_firstWidget : m_secondWidget;
    int res = container->insertWidget(index, w);

    if (visible) {
        container->show();
        container->setCurrentWidget(w);
        if (!isVisible()) {
            show();
        }
    } else {
        container->hide();
        updateVisible();
    }
    return res;
}

void CCoupleTabContainer::removeWidget(CCoupleTabTypes::Number num, QWidget *w) {
    QStackedWidget *container = (num == CCoupleTabTypes::First) ? m_firstWidget : m_secondWidget;
    bool isCurrent = container->currentWidget() == w;
    container->removeWidget(w);

    if (isCurrent && container->isVisible()) {
        container->hide();
        updateVisible();
    }
}

QWidget *CCoupleTabContainer::currentWidget(CCoupleTabTypes::Number num) const {
    QStackedWidget *container = (num == CCoupleTabTypes::First) ? m_firstWidget : m_secondWidget;
    return container->currentWidget();
}

int CCoupleTabContainer::currentIndex(CCoupleTabTypes::Number num) const {
    QStackedWidget *container = (num == CCoupleTabTypes::First) ? m_firstWidget : m_secondWidget;
    return container->currentIndex();
}

int CCoupleTabContainer::indexOf(CCoupleTabTypes::Number num, QWidget *w) const {
    QStackedWidget *container = (num == CCoupleTabTypes::First) ? m_firstWidget : m_secondWidget;
    return container->indexOf(w);
}

QWidget *CCoupleTabContainer::widget(CCoupleTabTypes::Number num, int index) const {
    QStackedWidget *container = (num == CCoupleTabTypes::First) ? m_firstWidget : m_secondWidget;
    return container->widget(index);
}

int CCoupleTabContainer::count(CCoupleTabTypes::Number num) const {
    QStackedWidget *container = (num == CCoupleTabTypes::First) ? m_firstWidget : m_secondWidget;
    return container->count();
}

void CCoupleTabContainer::setCurrentIndex(CCoupleTabTypes::Number num, int index) {
    QStackedWidget *container = (num == CCoupleTabTypes::First) ? m_firstWidget : m_secondWidget;
    return container->setCurrentIndex(index);
}

void CCoupleTabContainer::setCurrentWidget(CCoupleTabTypes::Number num, QWidget *w) {
    QStackedWidget *container = (num == CCoupleTabTypes::First) ? m_firstWidget : m_secondWidget;
    return container->setCurrentWidget(w);
}

void CCoupleTabContainer::setContainerVisible(CCoupleTabTypes::Number num, bool visible) {
    QStackedWidget *container = (num == CCoupleTabTypes::First) ? m_firstWidget : m_secondWidget;
    container->setVisible(visible);
    if (visible) {
        show();
    } else {
        updateVisible();
    }
}

void CCoupleTabContainer::updateVisible() {
    if (!m_firstWidget->isVisible() && !m_secondWidget->isVisible()) {
        hide();
    }
}
