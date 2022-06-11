#ifndef ANATIVETITLEBARLAYOUT_H
#define ANATIVETITLEBARLAYOUT_H

#include "ANativeBoxLayout.h"

class ANativeTitleBarLayout : public ANativeBoxLayout {
    Q_OBJECT
public:
    explicit ANativeTitleBarLayout(QWidget *parent = nullptr);
    ~ANativeTitleBarLayout();

public:
    A_LAYOUT_CREATE_INDEXER(iconWidget, setIconWidget, takeIconWidget, 0)
    A_LAYOUT_CREATE_INDEXER(menuWidget, setMenuWidget, takeMenuWidget, 1)
    A_LAYOUT_CREATE_INDEXER(labelWidget, setLabelWidget, takeLabelWidget, 2)
    A_LAYOUT_CREATE_INDEXER(minButtonWidget, setMinButtonWidget, takeMinButtonWidget, 3)
    A_LAYOUT_CREATE_INDEXER(maxButtonWidget, setMaxButtonWidget, takeMaxButtonWidget, 4)
    A_LAYOUT_CREATE_INDEXER(closeButtonWidget, setCloseButtonWidget, takeCloseButtonWidget, 5)

signals:
};

#endif // ANATIVETITLEBARLAYOUT_H
