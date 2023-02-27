#include "CDockCard.h"
#include "CDockTabBar.h"

#include <QApplication>
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QResizeEvent>
#include <QStyle>

#include "QMView.h"

CDockCard::CDockCard(QWidget *parent) : CLTabButton(parent) {
    init();
}

CDockCard::CDockCard(const QString &text, QWidget *parent) : CLTabButton(text, parent) {
    init();
}

CDockCard::CDockCard(const QIcon &icon, const QString &text, QWidget *parent)
    : CLTabButton(icon, text, parent) {
    init();
}

CDockCard::~CDockCard() {
}

void CDockCard::init() {
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

Qt::Orientation CDockCard::orientation() const {
    return m_orientation;
}

void CDockCard::setOrientation(Qt::Orientation orient) {
    m_orientation = orient;
    setSizePolicy((m_orientation == Qt::Horizontal) ? m_sizePolicyH : m_sizePolicyV);
    adjustSize();
}

QSize CDockCard::sizeHint() const {
    return (m_orientation == Qt::Horizontal) ? CTabButton::sizeHint() : CLTabButton::sizeHint();
}

QSize CDockCard::minimumSizeHint() const {
    return sizeHint();
}

int CDockCard::widthHint() const {
    return CTabButton::sizeHint().height();
}

QSize CDockCard::dragOffset() const {
    return m_dragOffset;
}

void CDockCard::setDragOffset(const QSize &dragOffset) {
    m_dragOffset = dragOffset;
    emit dragOffsetChanged();
}

QPixmap CDockCard::cardShot() const {
    QPixmap pixmap = QMView::createDeviceRenderPixmap(window()->windowHandle(), size());
    pixmap.fill(Qt::transparent);
    const_cast<CDockCard *>(this)->render(&pixmap);
    return pixmap;
}

CDockTabBar *CDockCard::tabBar() const {
    return qobject_cast<CDockTabBar *>(parentWidget());
}

QWidget *CDockCard::widget() const {
    return m_widget;
}

void CDockCard::setWidget(QWidget *widget) {
    m_widget = widget;
}

void CDockCard::mousePressEvent(QMouseEvent *event) {
    CLTabButton::mousePressEvent(event);
    m_dragPos = event->pos();
    m_readyDrag = true;
}

void CDockCard::mouseMoveEvent(QMouseEvent *event) {
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

void CDockCard::mouseReleaseEvent(QMouseEvent *event) {
    CLTabButton::mouseReleaseEvent(event);
    if (m_readyDrag) {
        m_readyDrag = false;
    }
}

void CDockCard::leaveEvent(QEvent *event) {
    Q_UNUSED(event)
    if (m_readyDrag) {
        m_readyDrag = false;
    }
}

void CDockCard::paintEvent(QPaintEvent *event) {
    return (m_orientation == Qt::Horizontal) ? CTabButton::paintEvent(event)
                                             : CLTabButton::paintEvent(event);
}

void CDockCard::resizeEvent(QResizeEvent *event) {
    return CLTabButton::resizeEvent(event);
}
