#ifndef VDIRTREEDRAGPROXY_H
#define VDIRTREEDRAGPROXY_H

#include "Basic/BaseDragProxy.h"

class VSpkTreeWidget;
class VSpkTreeDragLabel;
class QTreeWidgetItem;

class VSpkTreeDragProxy : public BaseDragProxy {
    Q_OBJECT
public:
    explicit VSpkTreeDragProxy(QObject *parent = nullptr);
    ~VSpkTreeDragProxy();

public:
    VSpkTreeWidget *treeWidget() const;
    void setTreeWidget(VSpkTreeWidget *treeWidget);

protected:
    VSpkTreeWidget *m_treeWidget;
    VSpkTreeDragLabel *m_dragger;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void handleDragStarted(QTreeWidgetItem *item);
    void handleTabDragMove();
    void handleTabDragOver();

signals:
    void dragOver(QTreeWidgetItem *item, const QPoint &pos);
};

#endif // VDIRTREEDRAGPROXY_H
