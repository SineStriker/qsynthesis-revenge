#include "CCoupleTabBarCard.h"
#include "CCoupleTabBar.h"

#include <QApplication>
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QResizeEvent>
#include <QStyle>

Q_UNIQUE_ID_DECLARE(CCoupleTabBarCard)

CCoupleTabBarCard::CCoupleTabBarCard(QWidget *parent) : CLTabButton(parent) {
    init();
}

CCoupleTabBarCard::CCoupleTabBarCard(const QString &text, QWidget *parent)
    : CLTabButton(text, parent) {
    init();
}

CCoupleTabBarCard::CCoupleTabBarCard(const QIcon &icon, const QString &text, QWidget *parent)
    : CLTabButton(icon, text, parent) {
    init();
}

CCoupleTabBarCard::~CCoupleTabBarCard() {
    destructId();
}

void CCoupleTabBarCard::init() {
    initId();

    setCheckable(true);
    setContextMenuPolicy(Qt::CustomContextMenu);

    m_widget = nullptr;

    m_dragOffset = QSize(10, 10);
    m_readyDrag = false;

    m_sizePolicyV = sizePolicy();
    m_sizePolicyH = QSizePolicy(m_sizePolicyV.verticalPolicy(), m_sizePolicyV.horizontalPolicy(),
                                m_sizePolicyV.controlType());
    setOrientation(Qt::Horizontal);
}

Qt::Orientation CCoupleTabBarCard::orientation() const {
    return m_orientation;
}

void CCoupleTabBarCard::setOrientation(Qt::Orientation orient) {
    m_orientation = orient;
    setSizePolicy((m_orientation == Qt::Horizontal) ? m_sizePolicyH : m_sizePolicyV);
    adjustSize();
}

QSize CCoupleTabBarCard::sizeHint() const {
    return (m_orientation == Qt::Horizontal) ? CTabButton::sizeHint() : CLTabButton::sizeHint();
}

QSize CCoupleTabBarCard::minimumSizeHint() const {
    return sizeHint();
}

int CCoupleTabBarCard::widthHint() const {
    return CTabButton::sizeHint().height();
}

QSize CCoupleTabBarCard::dragOffset() const {
    return m_dragOffset;
}

void CCoupleTabBarCard::setDragOffset(const QSize &dragOffset) {
    m_dragOffset = dragOffset;
    emit dragOffsetChanged();
}

QPixmap CCoupleTabBarCard::cardShot() const {
    QPixmap pixmap(size());
    pixmap.fill(Qt::transparent);
    const_cast<CCoupleTabBarCard *>(this)->render(&pixmap);
    return pixmap;
}

CCoupleTabBar *CCoupleTabBarCard::tabBar() const {
    return qobject_cast<CCoupleTabBar *>(parentWidget());
}

QWidget *CCoupleTabBarCard::widget() const {
    return m_widget;
}

void CCoupleTabBarCard::setWidget(QWidget *widget) {
    m_widget = widget;
}

void CCoupleTabBarCard::mousePressEvent(QMouseEvent *event) {
    CLTabButton::mousePressEvent(event);
    m_dragPos = event->pos();
    m_readyDrag = true;
}

void CCoupleTabBarCard::mouseMoveEvent(QMouseEvent *event) {
    CLTabButton::mouseMoveEvent(event);
    if (m_readyDrag) {
        QPoint pos = event->pos();
        if (qAbs(pos.x() - m_dragPos.x()) >= m_dragOffset.width() ||
            qAbs(pos.y() - m_dragPos.y()) >= m_dragOffset.height()) {
            m_readyDrag = false;
            emit startDrag(m_dragPos, cardShot());
        }
    }
}

void CCoupleTabBarCard::mouseReleaseEvent(QMouseEvent *event) {
    CLTabButton::mouseReleaseEvent(event);
    if (m_readyDrag) {
        m_readyDrag = false;
    }
}

void CCoupleTabBarCard::leaveEvent(QEvent *event) {
    Q_UNUSED(event)
    if (m_readyDrag) {
        m_readyDrag = false;
    }
}

void CCoupleTabBarCard::paintEvent(QPaintEvent *event) {
    return (m_orientation == Qt::Horizontal) ? CTabButton::paintEvent(event)
                                             : CLTabButton::paintEvent(event);
}

void CCoupleTabBarCard::resizeEvent(QResizeEvent *event) {
    return CLTabButton::resizeEvent(event);
}
