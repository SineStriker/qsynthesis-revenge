#include "VSpkTreeDragProxy.h"
#include "VSpkTreeDragLabel.h"
#include "../VSpkTreeWidget.h"

#include <QEvent>

VSpkTreeDragProxy::VSpkTreeDragProxy(QObject *parent) : BaseDragProxy(parent) {
    m_treeWidget = nullptr;
    m_dragger = nullptr;
}

VSpkTreeDragProxy::~VSpkTreeDragProxy() {
}

VSpkTreeWidget *VSpkTreeDragProxy::treeWidget() const {
    return m_treeWidget;
}

void VSpkTreeDragProxy::setTreeWidget(VSpkTreeWidget *treeWidget) {
    if (m_treeWidget) {
        disconnect(m_treeWidget, &VSpkTreeWidget::dragStarted, this,
                   &VSpkTreeDragProxy::handleDragStarted);
    }
    m_treeWidget = treeWidget;
    connect(m_treeWidget, &VSpkTreeWidget::dragStarted, this,
            &VSpkTreeDragProxy::handleDragStarted);
}

bool VSpkTreeDragProxy::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_dragger) {
        switch (event->type()) {
        case QEvent::MouseMove:
            handleTabDragMove();
            return true;
            break;
        case QEvent::FocusOut:
        case QEvent::MouseButtonRelease:
            handleTabDragOver();
            return true;
            break;
        default:
            break;
        }
    }
    return BaseDragProxy::eventFilter(obj, event);
}

void VSpkTreeDragProxy::handleDragStarted(QTreeWidgetItem *item) {
    if (!m_activeArea) {
        return;
    }
    m_dragger = new VSpkTreeDragLabel(m_activeArea);
    m_dragger->curItem = item;

    m_dragger->setIcon(item->icon(0));
    m_dragger->setIconSize(m_treeWidget->iconSize());
    m_dragger->setSpaceRatio(0.1);
    m_dragger->setText(item->text(0));

    m_dragger->adjustSize();
    m_dragger->setFocus();
    m_dragger->installEventFilter(this);
    m_dragger->grabMouse();

    handleTabDragMove();
    m_dragger->show();
}

void VSpkTreeDragProxy::handleTabDragMove() {
    if (!m_activeArea) {
        return;
    }
    QPoint pos = m_activeArea->mapFromGlobal(QCursor::pos());
    m_dragger->move(pos - m_dragger->currentPos);
}

void VSpkTreeDragProxy::handleTabDragOver() {
    QTreeWidgetItem *item = m_dragger->curItem;
    QPoint pos = QCursor::pos();

    m_dragger->removeEventFilter(this);
    m_dragger->releaseMouse();
    m_dragger->deleteLater(); // Don't delete directly
    m_dragger = nullptr;

    emit dragOver(item, pos);
}
