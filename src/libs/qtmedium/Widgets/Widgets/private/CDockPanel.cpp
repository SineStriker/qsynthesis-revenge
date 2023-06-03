#include "CDockPanel.h"

CDockPanel::CDockPanel(QWidget *parent) : QSplitter(parent) {
    init();
}

CDockPanel::CDockPanel(Qt::Orientation orient, QWidget *parent)
    : QSplitter(orient, parent) {
    init();
}

CDockPanel::~CDockPanel() {
}

void CDockPanel::init() {
    setChildrenCollapsible(false);

    m_firstWidget = new QStackedWidget();
    m_secondWidget = new QStackedWidget();

    QSplitter::addWidget(m_firstWidget);
    QSplitter::addWidget(m_secondWidget);

    m_firstWidget->hide();
    m_secondWidget->hide();
    updateVisibility();
}

int CDockPanel::addWidget(QM::Priority num, QWidget *w, bool visible) {
    return insertWidget(num, (num == QM::Primary) ? m_firstWidget->count() : m_secondWidget->count(),
                        w, visible);
}

int CDockPanel::insertWidget(QM::Priority num, int index, QWidget *w, bool visible) {
    QStackedWidget *container = (num == QM::Primary) ? m_firstWidget : m_secondWidget;
    int res = container->insertWidget(index, w);

    if (visible) {
        container->show();
        container->setCurrentWidget(w);
        if (!isVisible()) {
            show();
        }
    } /* else {
         container->hide();
         updateVisible();
     }*/
    return res;
}

void CDockPanel::removeWidget(QM::Priority num, QWidget *w) {
    QStackedWidget *container = (num == QM::Primary) ? m_firstWidget : m_secondWidget;
    bool isCurrent = container->currentWidget() == w;
    container->removeWidget(w);

    if (isCurrent && container->isVisible()) {
        container->hide();
        updateVisibility();
    }
}

QWidget *CDockPanel::currentWidget(QM::Priority num) const {
    QStackedWidget *container = (num == QM::Primary) ? m_firstWidget : m_secondWidget;
    return container->currentWidget();
}

int CDockPanel::currentIndex(QM::Priority num) const {
    QStackedWidget *container = (num == QM::Primary) ? m_firstWidget : m_secondWidget;
    return container->currentIndex();
}

int CDockPanel::indexOf(QM::Priority num, QWidget *w) const {
    QStackedWidget *container = (num == QM::Primary) ? m_firstWidget : m_secondWidget;
    return container->indexOf(w);
}

QWidget *CDockPanel::widget(QM::Priority num, int index) const {
    QStackedWidget *container = (num == QM::Primary) ? m_firstWidget : m_secondWidget;
    return container->widget(index);
}

int CDockPanel::count(QM::Priority num) const {
    QStackedWidget *container = (num == QM::Primary) ? m_firstWidget : m_secondWidget;
    return container->count();
}

void CDockPanel::setCurrentIndex(QM::Priority num, int index) {
    QStackedWidget *container = (num == QM::Primary) ? m_firstWidget : m_secondWidget;
    return container->setCurrentIndex(index);
}

void CDockPanel::setCurrentWidget(QM::Priority num, QWidget *w) {
    QStackedWidget *container = (num == QM::Primary) ? m_firstWidget : m_secondWidget;
    return container->setCurrentWidget(w);
}

void CDockPanel::setContainerVisible(QM::Priority num, bool visible) {
    QStackedWidget *container = (num == QM::Primary) ? m_firstWidget : m_secondWidget;
    container->setVisible(visible);
    if (visible) {
        show();
    } else {
        updateVisibility();
    }
}

void CDockPanel::updateVisibility() {
    if (!m_firstWidget->isVisible() && !m_secondWidget->isVisible()) {
        hide();
    }
}
