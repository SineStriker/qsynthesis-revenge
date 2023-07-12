#ifndef CHORUSKIT_CDOCKCARD_P_H
#define CHORUSKIT_CDOCKCARD_P_H

#include "QMView.h"

#include "../CDockCard.h"
#include "QMFloatingWindowHelper.h"

class CDockCardPrivate {
    Q_DECLARE_PUBLIC(CDockCard)
public:
    CDockCardPrivate();
    ~CDockCardPrivate();

    void init();

    QPixmap cardShot() const;

    CDockCard *q_ptr;

    CDockTabBar *m_tabBar;
    QRect oldGeometry;

    Qt::Orientation m_orientation;
    QSizePolicy m_sizePolicyH, m_sizePolicyV;

    QPoint m_dragPos;
    QSize m_dragOffset;

    bool m_readyDrag;

    QWidget *m_container;
    QWidget *m_widget;

    bool m_closing;
    CDockCard::ViewMode m_viewMode;

    QMFloatingWindowHelper *m_floatingHelper;

    static QMenu *createViewModeMenu(CDockCard *card);
};

#endif // CHORUSKIT_CDOCKCARD_P_H
