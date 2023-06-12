#ifndef CHORUSKIT_CDOCKCARD_P_H
#define CHORUSKIT_CDOCKCARD_P_H

#include "QMView.h"

#include "../CDockCard.h"

class CDockCardPrivate {
public:
    explicit CDockCardPrivate(CDockCard *q);
    ~CDockCardPrivate();

    void init();

    QPixmap cardShot() const;

    CDockCard *q;

    CDockTabBar *m_tabBar;

    Qt::Orientation m_orientation;
    QSizePolicy m_sizePolicyH, m_sizePolicyV;

    QPoint m_dragPos;
    QSize m_dragOffset;

    bool m_readyDrag;

    QWidget *m_container;
    QWidget *m_widget;

    bool m_closing;
    CDockCard::ViewMode m_viewMode;
};

#endif // CHORUSKIT_CDOCKCARD_P_H
