#include "CCoupleTabDoubleBar.h"

#include <QBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QStyle>

#include "EventHelper.h"

using namespace CCoupleTabTypes;

const char PROPERTY_HIGHLIGHT[] = "highlight";
const char PROPERTY_WIDTHHINT[] = "widthhint";

CCoupleTabDoubleBar::CCoupleTabDoubleBar(QWidget *parent) : QWidget(parent) {
    init();
}

CCoupleTabDoubleBar::~CCoupleTabDoubleBar() {
}

CardDirection CCoupleTabDoubleBar::cardDirection() const {
    return m_firstBar->cardDirection();
}

void CCoupleTabDoubleBar::setCardDirection(CardDirection cardDirection) {
    m_firstBar->setCardDirection(cardDirection);
    m_secondBar->setCardDirection(cardDirection);
    resetLayout();
}

void CCoupleTabDoubleBar::init() {
    qRegisterMetaType<Number>("CCoupleTabDoubleBar::Number");

    setAttribute(Qt::WA_StyledBackground);
    setProperty(PROPERTY_HIGHLIGHT, false);
    setProperty(PROPERTY_WIDTHHINT, 0);

    m_firstBar = new CCoupleTabBar();
    m_firstBar->setObjectName("first-bar");

    m_secondBar = new CCoupleTabBar(Backward);
    m_secondBar->setObjectName("second-bar");

    // Forwarding
    connect(m_firstBar, &CCoupleTabBar::cardAdded, this, &CCoupleTabDoubleBar::handleCardAdded);
    connect(m_firstBar, &CCoupleTabBar::cardRemoved, this, &CCoupleTabDoubleBar::handleCardRemoved);
    connect(m_firstBar, &CCoupleTabBar::cardToggled, this, &CCoupleTabDoubleBar::handleCardToggled);
    connect(m_secondBar, &CCoupleTabBar::cardAdded, this, &CCoupleTabDoubleBar::handleCardAdded);
    connect(m_secondBar, &CCoupleTabBar::cardRemoved, this,
            &CCoupleTabDoubleBar::handleCardRemoved);
    connect(m_secondBar, &CCoupleTabBar::cardToggled, this,
            &CCoupleTabDoubleBar::handleCardToggled);

    resetLayout();
}

Qt::Orientation CCoupleTabDoubleBar::orientation() const {
    return m_firstBar->orientation();
}

void CCoupleTabDoubleBar::setOrientation(Qt::Orientation orient) {
    if (orient != m_firstBar->orientation()) {
        m_firstBar->setOrientation(orient);
        m_secondBar->setOrientation(orient);
        resetLayout();
    }
}

bool CCoupleTabDoubleBar::highlight() const {
    return property(PROPERTY_HIGHLIGHT).toBool();
}

void CCoupleTabDoubleBar::setHighlight(bool highlight, int widthHint) {
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

int CCoupleTabDoubleBar::count() const {
    return m_firstBar->count() + m_secondBar->count();
}

CCoupleTabBar *CCoupleTabDoubleBar::firstBar() const {
    return m_firstBar;
}

CCoupleTabBar *CCoupleTabDoubleBar::secondBar() const {
    return m_secondBar;
}

QSize CCoupleTabDoubleBar::sizeHint() const {
    QSize sz = QWidget::sizeHint();
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

QSize CCoupleTabDoubleBar::minimumSizeHint() const {
    return sizeHint();
}

void CCoupleTabDoubleBar::resetLayout() {
    QBoxLayout *layout = static_cast<QBoxLayout *>(this->layout());
    if (layout) {
        delete layout;
    }

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

void CCoupleTabDoubleBar::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}

void CCoupleTabDoubleBar::handleCardAdded(CCoupleTabBarCard *card) {
    auto bar = qobject_cast<CCoupleTabBar *>(sender());
    if (bar == m_firstBar) {
        emit cardAdded(First, card);
    } else {
        emit cardAdded(Second, card);
    }
}

void CCoupleTabDoubleBar::handleCardRemoved(CCoupleTabBarCard *card) {
    auto bar = qobject_cast<CCoupleTabBar *>(sender());
    if (bar == m_firstBar) {
        emit cardRemoved(First, card);
    } else {
        emit cardRemoved(Second, card);
    }
}

void CCoupleTabDoubleBar::handleCardToggled(CCoupleTabBarCard *card) {
    auto bar = qobject_cast<CCoupleTabBar *>(sender());
    if (bar == m_firstBar) {
        emit cardToggled(First, card);
    } else {
        emit cardToggled(Second, card);
    }
}
