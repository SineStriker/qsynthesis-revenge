#include "VSpkTreeWidget.h"
#include "MathHelper.h"
#include "QMarginsImpl.h"

#include <private/qtreewidget_p.h>

#define DECODE_STYLE(VAR, VARIANT, TYPE)                                                           \
    {                                                                                              \
        QVariant var = VARIANT;                                                                    \
        if (var.convert(qMetaTypeId<TYPE>())) {                                                    \
            VAR = var.value<TYPE>();                                                               \
        }                                                                                          \
    }

VSpkTreeWidget::VSpkTreeWidget(QWidget *parent) : CTreeWidget(parent) {
    setHeaderHidden(true);

    m_delegate = new VTreeItemDelegate(this);
    setItemDelegate(m_delegate);
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

void VSpkTreeWidget::setStyleData(const QTypeList &list) {
    if (list.size() >= 6) {
        int i = 0;
        DECODE_STYLE(m_delegate->m_langTagHeight, list.at(i++), QPixelSize);
        DECODE_STYLE(m_delegate->m_itemTextType, list.at(i++), QTypeFace);
        DECODE_STYLE(m_delegate->m_langTextType, list.at(i++), QTypeFace);
        DECODE_STYLE(m_delegate->m_langTagMargins, list.at(i++), QMargins);
        DECODE_STYLE(m_delegate->m_margins, list.at(i++), QMargins);
        DECODE_STYLE(m_delegate->m_colors, list.at(i++), QColorList);

        update();
        emit styleDataChanged();
    }
}

QTypeList VSpkTreeWidget::styleData() const {
    return {
        QVariant::fromValue(m_delegate->m_langTagHeight),
        QVariant::fromValue(m_delegate->m_itemTextType),
        QVariant::fromValue(m_delegate->m_langTextType),
        QVariant::fromValue(m_delegate->m_langTagMargins),
        QVariant::fromValue(m_delegate->m_margins),
        QVariant::fromValue(m_delegate->m_colors),
    };
}
