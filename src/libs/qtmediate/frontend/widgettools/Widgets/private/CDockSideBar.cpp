#include "CDockSideBar.h"

#include "CDockFrame.h"

#include <QBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QStyle>

static const char PROPERTY_HIGHLIGHT[] = "highlight";
static const char PROPERTY_WIDTHHINT[] = "widthhint";

CDockSideBar::CDockSideBar(QWidget *parent) : QFrame(parent) {
    init();
}

CDockSideBar::~CDockSideBar() {
}

QM::Direction CDockSideBar::cardDirection() const {
    return m_firstBar->cardDirection();
}

void CDockSideBar::setCardDirection(QM::Direction cardDirection) {
    m_firstBar->setCardDirection(cardDirection);
    m_secondBar->setCardDirection(cardDirection);
    resetLayout();
}

void CDockSideBar::init() {
    setAttribute(Qt::WA_StyledBackground);

    setProperty(PROPERTY_HIGHLIGHT, false);
    setProperty(PROPERTY_WIDTHHINT, 0);

    m_firstBar = new CDockTabBar();
    m_firstBar->setObjectName("first-bar");

    m_secondBar = new CDockTabBar(QM::Backward);
    m_secondBar->setObjectName("second-bar");

    // Forwarding
    connect(m_firstBar, &CDockTabBar::cardAdded, this, &CDockSideBar::_q_cardAdded);
    connect(m_firstBar, &CDockTabBar::cardRemoved, this, &CDockSideBar::_q_cardRemoved);
    connect(m_firstBar, &CDockTabBar::cardToggled, this, &CDockSideBar::_q_cardToggled);
    connect(m_firstBar, &CDockTabBar::cardViewModeChanged, this, &CDockSideBar::_q_cardViewModeChanged);

    connect(m_secondBar, &CDockTabBar::cardAdded, this, &CDockSideBar::_q_cardAdded);
    connect(m_secondBar, &CDockTabBar::cardRemoved, this, &CDockSideBar::_q_cardRemoved);
    connect(m_secondBar, &CDockTabBar::cardToggled, this, &CDockSideBar::_q_cardToggled);
    connect(m_secondBar, &CDockTabBar::cardViewModeChanged, this, &CDockSideBar::_q_cardViewModeChanged);

    resetLayout();
}

Qt::Orientation CDockSideBar::orientation() const {
    return m_firstBar->orientation();
}

void CDockSideBar::setOrientation(Qt::Orientation orient) {
    if (orient != m_firstBar->orientation()) {
        m_firstBar->setOrientation(orient);
        m_secondBar->setOrientation(orient);
        resetLayout();
    }
}

Qt::Edge CDockSideBar::edge() const {
    if (orientation() == Qt::Horizontal) {
        if (cardDirection() == QM::Backward) {
            return Qt::TopEdge;
        } else {
            return Qt::BottomEdge;
        }
    } else {

        if (cardDirection() == QM::Backward) {
            return Qt::LeftEdge;
        } else {
            return Qt::RightEdge;
        }
    }
}

bool CDockSideBar::highlight() const {
    return property(PROPERTY_HIGHLIGHT).toBool();
}

void CDockSideBar::setHighlight(bool highlight, int widthHint) {
    if (this->highlight() != highlight) {
        setProperty(PROPERTY_HIGHLIGHT, highlight);
        style()->polish(this);
    }
    if (highlight) {
        setProperty(PROPERTY_WIDTHHINT, widthHint);
    } else {
        setProperty(PROPERTY_WIDTHHINT, 0);
    }
    updateGeometry();
}

int CDockSideBar::count() const {
    return m_firstBar->count() + m_secondBar->count();
}

CDockTabBar *CDockSideBar::firstBar() const {
    return m_firstBar;
}

CDockTabBar *CDockSideBar::secondBar() const {
    return m_secondBar;
}

QSize CDockSideBar::sizeHint() const {
    QSize sz = QFrame::sizeHint();
    if (highlight()) {
        int wh = property(PROPERTY_WIDTHHINT).toInt();
        if (orientation() == Qt::Horizontal && sz.height() < wh) {
            sz.setHeight(wh);
        } else if (orientation() == Qt::Vertical && sz.width() < wh) {
            sz.setWidth(wh);
        }
        return sz;
    }
    return sz;
}

QSize CDockSideBar::minimumSizeHint() const {
    return sizeHint();
}

CDockFrame *CDockSideBar::frame() const {
    QWidget *w = const_cast<CDockSideBar *>(this);
    while (w) {
        auto f = qobject_cast<CDockFrame *>(w);
        if (f) {
            return f;
        }
        w = parentWidget();
    }
    return nullptr;
}

void CDockSideBar::resetLayout() {
    auto layout = static_cast<QBoxLayout *>(this->layout());
    delete layout;

    if (orientation() == Qt::Horizontal) {
        layout = new QHBoxLayout();
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    } else {
        layout = new QVBoxLayout();
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    }

    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(m_firstBar);
    layout->addStretch();
    layout->addWidget(m_secondBar);

    setLayout(layout);
}

void CDockSideBar::mousePressEvent(QMouseEvent *event) {
    QFrame::mousePressEvent(event);
}

void CDockSideBar::_q_cardAdded(CDockCard *card) {
    auto bar = qobject_cast<CDockTabBar *>(sender());
    if (bar == m_firstBar) {
        emit cardAdded(QM::Primary, card);
    } else {
        emit cardAdded(QM::Secondary, card);
    }
}

void CDockSideBar::_q_cardRemoved(CDockCard *card) {
    auto bar = qobject_cast<CDockTabBar *>(sender());
    if (bar == m_firstBar) {
        emit cardRemoved(QM::Primary, card);
    } else {
        emit cardRemoved(QM::Secondary, card);
    }
}

void CDockSideBar::_q_cardToggled(CDockCard *card) {
    auto bar = qobject_cast<CDockTabBar *>(sender());
    if (bar == m_firstBar) {
        emit cardToggled(QM::Primary, card);
    } else {
        emit cardToggled(QM::Secondary, card);
    }
}

void CDockSideBar::_q_cardViewModeChanged(CDockCard *card, CDockCard::ViewMode oldViewMode) {
    auto bar = qobject_cast<CDockTabBar *>(sender());
    if (bar == m_firstBar) {
        emit cardViewModeChanged(QM::Primary, card, oldViewMode);
    } else {
        emit cardViewModeChanged(QM::Secondary, card, oldViewMode);
    }
}
