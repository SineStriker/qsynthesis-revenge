#ifndef CTREEWIDGET_H
#define CTREEWIDGET_H

#include <QTreeWidget>

#include "QMWidgetsGlobal.h"

class QMWIDGETS_EXPORT CTreeWidget : public QTreeWidget {
    Q_OBJECT
public:
    explicit CTreeWidget(QWidget *parent = nullptr);
    ~CTreeWidget();

public:
    static QTreeWidgetItem *index2item(const QModelIndex &index);

    void setItemExpandedRecursively(QTreeWidgetItem *item, bool expanded);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void itemClickedEx(QTreeWidgetItem *item, int column, Qt::MouseButton button);
};

#endif // CTREEWIDGET_H
