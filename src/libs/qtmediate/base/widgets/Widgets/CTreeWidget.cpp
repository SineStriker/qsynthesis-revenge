#include "CTreeWidget.h"

#include <private/qtreewidget_p.h>

CTreeWidget::CTreeWidget(QWidget *parent) : QTreeWidget(parent) {
    setFont(qApp->font());
}

CTreeWidget::~CTreeWidget() {
}

QTreeWidgetItem *CTreeWidget::index2item(const QModelIndex &index) {
    if (!index.isValid()) {
        return nullptr;
    }
    return static_cast<QTreeWidgetItem *>(index.internalPointer());
}

void CTreeWidget::setItemExpandedRecursively(QTreeWidgetItem *item, bool expanded) {
    std::list<QTreeWidgetItem *> queue;
    queue.push_back(item);
    while (!queue.empty()) {
        auto curItem = queue.back();
        queue.pop_back();
        curItem->setExpanded(expanded);
        for (int i = 0; i < curItem->childCount(); ++i) {
            auto childItem = curItem->child(i);
            queue.push_back(childItem);
        }
    }
}

void CTreeWidget::mouseReleaseEvent(QMouseEvent *event) {
    QTreeWidget::mouseReleaseEvent(event);

    auto d = static_cast<QTreeWidgetPrivate *>(d_ptr.data());
    QPoint pos = event->pos();
    QPersistentModelIndex index = indexAt(pos);
    bool click = (index == d->pressedIndex && index.isValid());

    if (click) {
        auto item = index2item(index);
        emit itemClickedEx(item, index.column(), event->button());
    }
}
