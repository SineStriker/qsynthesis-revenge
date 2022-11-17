#ifndef VDIRTREEWIDGET_H
#define VDIRTREEWIDGET_H

#include "CTreeWidget.h"
#include "QSvgUri.h"

class VSpkTreeWidget : public CTreeWidget {
    Q_OBJECT
public:
    explicit VSpkTreeWidget(QWidget *parent = nullptr);
    ~VSpkTreeWidget();

protected:
    QPoint lastPressPos;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void dropEvent(QDropEvent *event) override;

signals:
    void iconChanged();
    void dragStarted(QTreeWidgetItem *item);

    void itemDroped(QTreeWidgetItem *item, QTreeWidgetItem *oldParent);
};

#endif // VDIRTREEWIDGET_H
