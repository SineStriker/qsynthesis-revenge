#include "CDockCard_p.h"

CDockCardPrivate::CDockCardPrivate(CDockCard *q) : q(q) {
    m_closing = false;

    m_container = nullptr;
    m_widget = nullptr;
    m_viewMode = CDockCard::DockPinned;

    m_dragOffset = QSize(10, 10);
    m_readyDrag = false;

    m_sizePolicyV = q->sizePolicy();
    m_sizePolicyH =
        QSizePolicy(m_sizePolicyV.verticalPolicy(), m_sizePolicyV.horizontalPolicy(), m_sizePolicyV.controlType());

    m_tabBar = nullptr;
}

CDockCardPrivate::~CDockCardPrivate() {
    if (!m_widget->parentWidget()) {
        m_widget->deleteLater();
    }
    if (!m_container->parentWidget()) {
        m_container->deleteLater();
    }
}
void CDockCardPrivate::init() {
    q->setCheckable(true);
    q->setContextMenuPolicy(Qt::DefaultContextMenu);
    q->setOrientation(Qt::Horizontal); // orientation
}

QPixmap CDockCardPrivate::cardShot() const {
    QPixmap pixmap = QMView::createDeviceRenderPixmap(q->window()->windowHandle(), q->size());
    pixmap.fill(Qt::transparent);
    q->render(&pixmap);
    return pixmap;
}
