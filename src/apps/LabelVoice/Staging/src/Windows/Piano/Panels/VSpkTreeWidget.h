#ifndef VDIRTREEWIDGET_H
#define VDIRTREEWIDGET_H

#include "CTreeWidget.h"
#include "QSvgUri.h"

#include "QTypeList.h"
#include "private/VTreeItemDelegate.h"

class VSpkTreeWidget : public CTreeWidget {
    Q_OBJECT
    Q_PROPERTY(QTypeList styleData READ styleData WRITE setStyleData NOTIFY styleDataChanged)
public:
    explicit VSpkTreeWidget(QWidget *parent = nullptr);
    ~VSpkTreeWidget();

protected:
    QPoint lastPressPos;

    VTreeItemDelegate *m_delegate;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void dropEvent(QDropEvent *event) override;

    void setStyleData(const QTypeList &list);
    QTypeList styleData() const;

signals:
    void iconChanged();
    void dragStarted(QTreeWidgetItem *item);

    void itemDroped(QTreeWidgetItem *item, QTreeWidgetItem *oldParent);

    void styleDataChanged();
};

#endif // VDIRTREEWIDGET_H
