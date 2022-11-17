#ifndef VDIRTREEDRAGLABEL_H
#define VDIRTREEDRAGLABEL_H

#include "CTabButton.h"

class QTreeWidgetItem;

class VSpkTreeDragLabel : public CTabButton {
    Q_OBJECT
public:
    explicit VSpkTreeDragLabel(QWidget *parent = nullptr);
    ~VSpkTreeDragLabel();

public:
    QPoint currentPos;
    QTreeWidgetItem *curItem;

signals:
};

#endif // VDIRTREEDRAGLABEL_H
