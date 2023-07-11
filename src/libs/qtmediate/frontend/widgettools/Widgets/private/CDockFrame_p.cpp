#include "CDockFrame_p.h"

#include <QMenu>

#include "CDockCard_p.h"
#include "CDockToolWindow.h"

CDockFramePrivate::CDockFramePrivate() {
}

CDockFramePrivate::~CDockFramePrivate() {
}

void CDockFramePrivate::init() {
    Q_Q(CDockFrame);

    q->setAttribute(Qt::WA_StyledBackground);

    m_layout = new QGridLayout();
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    m_leftBar = new CDockSideBar();
    m_leftBar->setObjectName("left-bar");
    m_topBar = new CDockSideBar();
    m_topBar->setObjectName("top-bar");
    m_rightBar = new CDockSideBar();
    m_rightBar->setObjectName("right-bar");
    m_bottomBar = new CDockSideBar();
    m_bottomBar->setObjectName("bottom-bar");

    m_leftBar->setOrientation(Qt::Vertical);
    m_topBar->setOrientation(Qt::Horizontal);
    m_rightBar->setOrientation(Qt::Vertical);
    m_bottomBar->setOrientation(Qt::Horizontal);

    m_leftBar->setCardDirection(QM::Backward);
    m_topBar->setCardDirection(QM::Backward);
    m_rightBar->setCardDirection(QM::Forward);
    m_bottomBar->setCardDirection(QM::Forward);

    m_leftPanel = new CDockPanel(Qt::Vertical);
    m_topPanel = new CDockPanel(Qt::Horizontal);
    m_rightPanel = new CDockPanel(Qt::Vertical);
    m_bottomPanel = new CDockPanel(Qt::Horizontal);

    m_centralContainer = new QStackedWidget();

    /*
     *   0   1   2
     * 0   t t t
     *   l · · · r
     *
     * 1 l · * · r
     *
     *   l · · · r
     * 2   b b b
     *
     */

    m_horizontalSplitter = new CDockPanelSplitter(Qt::Horizontal);
    m_horizontalSplitter->setChildrenCollapsible(false);
    m_horizontalSplitter->addWidget(m_leftPanel);
    m_horizontalSplitter->addWidget(m_centralContainer);
    m_horizontalSplitter->addWidget(m_rightPanel);

    m_horizontalSplitter->setStretchFactor(0, 0);
    m_horizontalSplitter->setStretchFactor(1, 1);
    m_horizontalSplitter->setStretchFactor(2, 0);

    m_verticalSplitter = new CDockPanelSplitter(Qt::Vertical);
    m_verticalSplitter->setChildrenCollapsible(false);
    m_verticalSplitter->addWidget(m_topPanel);
    m_verticalSplitter->addWidget(m_horizontalSplitter);
    m_verticalSplitter->addWidget(m_bottomPanel);

    m_verticalSplitter->setStretchFactor(0, 0);
    m_verticalSplitter->setStretchFactor(1, 1);
    m_verticalSplitter->setStretchFactor(2, 0);

    m_layout->addWidget(m_leftBar, 1, 0);
    m_layout->addWidget(m_topBar, 0, 1);
    m_layout->addWidget(m_rightBar, 1, 2);
    m_layout->addWidget(m_bottomBar, 2, 1);

    m_layout->addWidget(m_verticalSplitter, 1, 1);

    q->setLayout(m_layout);

    m_dragProxy = new CDockTabDragProxy(this);
    m_dragProxy->setFrame(q);

    m_leftBar->installEventFilter(this);

    connect(m_leftBar, &CDockSideBar::cardAdded, this, &CDockFramePrivate::_q_cardAdded);
    connect(m_leftBar, &CDockSideBar::cardRemoved, this, &CDockFramePrivate::_q_cardRemoved);
    connect(m_leftBar, &CDockSideBar::cardToggled, this, &CDockFramePrivate::_q_cardToggled);
    connect(m_leftBar, &CDockSideBar::cardViewModeChanged, this, &CDockFramePrivate::_q_cardViewModeChanged);

    connect(m_topBar, &CDockSideBar::cardAdded, this, &CDockFramePrivate::_q_cardAdded);
    connect(m_topBar, &CDockSideBar::cardRemoved, this, &CDockFramePrivate::_q_cardRemoved);
    connect(m_topBar, &CDockSideBar::cardToggled, this, &CDockFramePrivate::_q_cardToggled);
    connect(m_topBar, &CDockSideBar::cardViewModeChanged, this, &CDockFramePrivate::_q_cardViewModeChanged);

    connect(m_rightBar, &CDockSideBar::cardAdded, this, &CDockFramePrivate::_q_cardAdded);
    connect(m_rightBar, &CDockSideBar::cardRemoved, this, &CDockFramePrivate::_q_cardRemoved);
    connect(m_rightBar, &CDockSideBar::cardToggled, this, &CDockFramePrivate::_q_cardToggled);
    connect(m_rightBar, &CDockSideBar::cardViewModeChanged, this, &CDockFramePrivate::_q_cardViewModeChanged);

    connect(m_bottomBar, &CDockSideBar::cardAdded, this, &CDockFramePrivate::_q_cardAdded);
    connect(m_bottomBar, &CDockSideBar::cardRemoved, this, &CDockFramePrivate::_q_cardRemoved);
    connect(m_bottomBar, &CDockSideBar::cardToggled, this, &CDockFramePrivate::_q_cardToggled);
    connect(m_bottomBar, &CDockSideBar::cardViewModeChanged, this, &CDockFramePrivate::_q_cardViewModeChanged);

    orgHSizes = m_horizontalSplitter->sizes();
    orgVSizes = m_verticalSplitter->sizes();
}

void CDockFramePrivate::_q_cardAdded(QM::Priority number, CDockCard *card) {
    auto doubleBar = qobject_cast<CDockSideBar *>(sender());
    CDockPanel *container;
    if (doubleBar == m_leftBar) {
        container = m_leftPanel;
    } else if (doubleBar == m_topBar) {
        container = m_topPanel;
    } else if (doubleBar == m_rightBar) {
        container = m_rightPanel;
    } else {
        container = m_bottomPanel;
    }
    container->addWidget(number, card->container(), card->dockVisible());
}

void CDockFramePrivate::_q_cardRemoved(QM::Priority number, CDockCard *card) {
    auto doubleBar = qobject_cast<CDockSideBar *>(sender());
    CDockPanel *container;
    if (doubleBar == m_leftBar) {
        container = m_leftPanel;
    } else if (doubleBar == m_topBar) {
        container = m_topPanel;
    } else if (doubleBar == m_rightBar) {
        container = m_rightPanel;
    } else {
        container = m_bottomPanel;
    }
    container->removeWidget(number, card->container());
}

void CDockFramePrivate::_q_cardToggled(QM::Priority number, CDockCard *card) {
    Q_Q(CDockFrame);

    auto doubleBar = qobject_cast<CDockSideBar *>(sender());
    CDockPanel *container;
    Qt::Edge edge;
    if (doubleBar == m_leftBar) {
        edge = Qt::LeftEdge;
        container = m_leftPanel;
    } else if (doubleBar == m_topBar) {
        edge = Qt::TopEdge;
        container = m_topPanel;
    } else if (doubleBar == m_rightBar) {
        edge = Qt::RightEdge;
        container = m_rightPanel;
    } else {
        edge = Qt::BottomEdge;
        container = m_bottomPanel;
    }

    bool visible = card->isChecked();
    if (card->viewMode() == CDockCard::DockPinned) {
        container->setContainerVisible(number, visible);
        if (visible) {
            container->setCurrentWidget(number, card->container());
        }
    } else {
        card->widget()->setVisible(visible);
    }
    emit q->cardToggled(edge, number, card);
}

void CDockFramePrivate::_q_cardViewModeChanged(QM::Priority number, CDockCard *card, CDockCard::ViewMode oldViewMode) {
    auto doubleBar = qobject_cast<CDockSideBar *>(sender());
    CDockPanel *container;
    if (doubleBar == m_leftBar) {
        container = m_leftPanel;
    } else if (doubleBar == m_topBar) {
        container = m_topPanel;
    } else if (doubleBar == m_rightBar) {
        container = m_rightPanel;
    } else {
        container = m_bottomPanel;
    }

    bool visible = card->isChecked();
    if (visible) {
        if (oldViewMode == CDockCard::DockPinned) {
            container->setContainerVisible(number, false);
        } else if (card->viewMode() == CDockCard::DockPinned) {
            container->setContainerVisible(number, true);
            container->setCurrentWidget(number, card->container());

            // We must refresh its layout
            card->container()->layout()->invalidate();
        }
    }
}