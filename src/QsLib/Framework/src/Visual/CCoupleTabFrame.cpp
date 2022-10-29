#include "CCoupleTabFrame.h"

#include <QEvent>

using namespace CCoupleTabTypes;

CCoupleTabFrame::CCoupleTabFrame(QWidget *parent) : QFrame(parent) {
    init();
}

CCoupleTabFrame::~CCoupleTabFrame() {
}

void CCoupleTabFrame::init() {
    setAttribute(Qt::WA_StyledBackground);
    m_dragAreaSize = QSize(30, 30);

    m_layout = new QGridLayout();
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    m_leftBar = new CCoupleTabDoubleBar();
    m_leftBar->setObjectName("left-bar");
    m_topBar = new CCoupleTabDoubleBar();
    m_topBar->setObjectName("top-bar");
    m_rightBar = new CCoupleTabDoubleBar();
    m_rightBar->setObjectName("right-bar");
    m_bottomBar = new CCoupleTabDoubleBar();
    m_bottomBar->setObjectName("bottom-bar");

    m_leftBar->setOrientation(Qt::Vertical);
    m_topBar->setOrientation(Qt::Horizontal);
    m_rightBar->setOrientation(Qt::Vertical);
    m_bottomBar->setOrientation(Qt::Horizontal);

    m_leftBar->setCardDirection(CCoupleTabTypes::BottomToTop);
    m_topBar->setCardDirection(CCoupleTabTypes::BottomToTop);
    m_rightBar->setCardDirection(CCoupleTabTypes::TopToBottom);
    m_bottomBar->setCardDirection(CCoupleTabTypes::TopToBottom);

    m_leftPanel = new CCoupleTabContainer(Qt::Vertical);
    m_topPanel = new CCoupleTabContainer(Qt::Horizontal);
    m_rightPanel = new CCoupleTabContainer(Qt::Vertical);
    m_bottomPanel = new CCoupleTabContainer(Qt::Horizontal);

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

    m_horizontalSplitter = new CCoupleTabSplitter(Qt::Horizontal);
    m_horizontalSplitter->setChildrenCollapsible(false);
    m_horizontalSplitter->addWidget(m_leftPanel);
    m_horizontalSplitter->addWidget(m_centralContainer);
    m_horizontalSplitter->addWidget(m_rightPanel);

    m_horizontalSplitter->setStretchFactor(0, 0);
    m_horizontalSplitter->setStretchFactor(1, 1);
    m_horizontalSplitter->setStretchFactor(2, 0);

    m_verticalSplitter = new CCoupleTabSplitter(Qt::Vertical);
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

    setLayout(m_layout);

    m_dragProxy = new CCoupleTabDragProxy(this);
    m_dragProxy->setFrame(this);

    m_leftBar->installEventFilter(this);

    connect(m_leftBar, &CCoupleTabDoubleBar::cardAdded, this, &CCoupleTabFrame::handleCardAdded);
    connect(m_leftBar, &CCoupleTabDoubleBar::cardRemoved, this,
            &CCoupleTabFrame::handleCardRemoved);
    connect(m_leftBar, &CCoupleTabDoubleBar::cardToggled, this,
            &CCoupleTabFrame::handleCardToggled);

    connect(m_topBar, &CCoupleTabDoubleBar::cardAdded, this, &CCoupleTabFrame::handleCardAdded);
    connect(m_topBar, &CCoupleTabDoubleBar::cardRemoved, this, &CCoupleTabFrame::handleCardRemoved);
    connect(m_topBar, &CCoupleTabDoubleBar::cardToggled, this, &CCoupleTabFrame::handleCardToggled);

    connect(m_rightBar, &CCoupleTabDoubleBar::cardAdded, this, &CCoupleTabFrame::handleCardAdded);
    connect(m_rightBar, &CCoupleTabDoubleBar::cardRemoved, this,
            &CCoupleTabFrame::handleCardRemoved);
    connect(m_rightBar, &CCoupleTabDoubleBar::cardToggled, this,
            &CCoupleTabFrame::handleCardToggled);

    connect(m_bottomBar, &CCoupleTabDoubleBar::cardAdded, this, &CCoupleTabFrame::handleCardAdded);
    connect(m_bottomBar, &CCoupleTabDoubleBar::cardRemoved, this,
            &CCoupleTabFrame::handleCardRemoved);
    connect(m_bottomBar, &CCoupleTabDoubleBar::cardToggled, this,
            &CCoupleTabFrame::handleCardToggled);
}

QWidget *CCoupleTabFrame::widget() const {
    return (m_centralContainer->count() == 0) ? nullptr : m_centralContainer->widget(0);
}

void CCoupleTabFrame::setWidget(QWidget *w) {
    if (m_centralContainer->count() > 0) {
        delete takeWidget();
    }
    if (w) {
        m_centralContainer->addWidget(w);
    }
}

QWidget *CCoupleTabFrame::takeWidget() {
    if (m_centralContainer->count() > 0) {
        auto w = m_centralContainer->widget(0);
        m_centralContainer->removeWidget(w);
        return w;
    }
    return nullptr;
}

CCoupleTabDoubleBar *CCoupleTabFrame::leftBar() const {
    return m_leftBar;
}

CCoupleTabDoubleBar *CCoupleTabFrame::topBar() const {
    return m_topBar;
}

CCoupleTabDoubleBar *CCoupleTabFrame::rightBar() const {
    return m_rightBar;
}

CCoupleTabDoubleBar *CCoupleTabFrame::bottomBar() const {
    return m_bottomBar;
}

QSize CCoupleTabFrame::dragAreaSize() const {
    return m_dragAreaSize;
}

void CCoupleTabFrame::setDragAreaSize(const QSize &dragAreaSize) {
    m_dragAreaSize = dragAreaSize;
    emit dragAreaSizeChanged();
}

bool CCoupleTabFrame::barVisible() const {
    return m_leftBar->isVisible();
}

void CCoupleTabFrame::setBarVisible(bool visible) {
    m_leftBar->setVisible(visible);
    m_topBar->setVisible(visible);
    m_rightBar->setVisible(visible);
    m_bottomBar->setVisible(visible);
}

void CCoupleTabFrame::toggleBarVisible() {
    setBarVisible(!barVisible());
}

bool CCoupleTabFrame::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_leftBar) {
        switch (event->type()) {
        case QEvent::Hide:
            if (!isHidden()) {
                break;
            }
        case QEvent::Show:
            emit barVisibleToggled(event->type() == QEvent::Show);
            break;
        default:
            break;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void CCoupleTabFrame::handleCardAdded(CCoupleTabTypes::Number number, CCoupleTabBarCard *card) {
    auto doubleBar = qobject_cast<CCoupleTabDoubleBar *>(sender());
    CCoupleTabContainer *container;
    if (doubleBar == m_leftBar) {
        container = m_leftPanel;
    } else if (doubleBar == m_topBar) {
        container = m_topPanel;
    } else if (doubleBar == m_rightBar) {
        container = m_rightPanel;
    } else {
        container = m_bottomPanel;
    }
    container->addWidget(number, card->widget(), card->isChecked());
}

void CCoupleTabFrame::handleCardRemoved(CCoupleTabTypes::Number number, CCoupleTabBarCard *card) {
    auto doubleBar = qobject_cast<CCoupleTabDoubleBar *>(sender());
    CCoupleTabContainer *container;
    if (doubleBar == m_leftBar) {
        container = m_leftPanel;
    } else if (doubleBar == m_topBar) {
        container = m_topPanel;
    } else if (doubleBar == m_rightBar) {
        container = m_rightPanel;
    } else {
        container = m_bottomPanel;
    }
    container->removeWidget(number, card->widget());
}

void CCoupleTabFrame::handleCardToggled(Number number, CCoupleTabBarCard *card) {
    auto doubleBar = qobject_cast<CCoupleTabDoubleBar *>(sender());
    CCoupleTabContainer *container;
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
    container->setContainerVisible(number, visible);
    if (visible) {
        container->setCurrentWidget(number, card->widget());
    }
    emit cardToggled(edge, number, card);
}
