#include "VSpkTreeWidget.h"
#include "MathHelper.h"

#include <private/qtreewidget_p.h>

VSpkTreeWidget::VSpkTreeWidget(QWidget *parent) : CTreeWidget(parent) {
    setHeaderHidden(true);
}

VSpkTreeWidget::~VSpkTreeWidget() {
}

void VSpkTreeWidget::mousePressEvent(QMouseEvent *event) {
    lastPressPos = event->pos();
    CTreeWidget::mousePressEvent(event);
}

void VSpkTreeWidget::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
    //    if (state() == ExpandingState || state() == CollapsingState) {
    //        return;
    //    }
    //    auto d = static_cast<QTreeWidgetPrivate *>(d_ptr.data());
    //    QPoint pos = event->pos();
    //    QPersistentModelIndex index = indexAt(lastPressPos);

    //    if (index == d->pressedIndex && index.isValid()) {
    //        if (Math::euclideanDistance(pos, lastPressPos) >= 10) {
    //            QMouseEvent e(QEvent::MouseButtonRelease, pos, Qt::LeftButton, Qt::NoButton,
    //                          Qt::NoModifier);
    //            QApplication::sendEvent(this, &e);

    //            auto item = index2item(index);
    //            emit dragStarted(item);
    //            return;
    //        }
    //    }
    CTreeWidget::mouseMoveEvent(event);
}

void VSpkTreeWidget::mouseReleaseEvent(QMouseEvent *event) {
    CTreeWidget::mouseReleaseEvent(event);
}

void VSpkTreeWidget::dropEvent(QDropEvent *event) {
    QList<QTreeWidgetItem *> kids = this->selectedItems();

    /* row number before the drag - initial position */
    if (kids.isEmpty())
        return;

    QTreeWidgetItem *item = kids.front();
    QTreeWidgetItem *parent = item->parent();

    /* perform the default implementation */
    QTreeWidget::dropEvent(event);

    QTreeWidgetItem *dest = item->parent();

    if (dest == parent) {
        return;
    }

    /* emit signal about the move */
    emit itemDroped(item, parent);
}
