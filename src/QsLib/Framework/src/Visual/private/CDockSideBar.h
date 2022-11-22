#ifndef __CDOCKSIDEBAR_H__
#define __CDOCKSIDEBAR_H__

#include <QFrame>

#include "CDockTabBar.h"
#include "qsframework_global.h"

class CDockTabDragProxy;

class QSFRAMEWORK_API CDockSideBar : public QFrame {
    Q_OBJECT
public:
    explicit CDockSideBar(QWidget *parent = nullptr);
    ~CDockSideBar();

    friend class CDockTabBar;

private:
    void init();

public:
    Qs::Direction cardDirection() const;
    void setCardDirection(Qs::Direction cardDirection);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orient);

    bool highlight() const;
    void setHighlight(bool highlight, int widthHint = 0);

    int count() const;

    CDockTabBar *firstBar() const;
    CDockTabBar *secondBar() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    CDockTabBar *m_firstBar;
    CDockTabBar *m_secondBar;

    void resetLayout();

    void mousePressEvent(QMouseEvent *event) override;

private:
    void _q_cardAdded(CDockCard *card);
    void _q_cardRemoved(CDockCard *card);
    void _q_cardToggled(CDockCard *card);

signals:
    void cardAdded(Qs::Priority number, CDockCard *card);
    void cardRemoved(Qs::Priority number, CDockCard *card);
    void cardToggled(Qs::Priority number, CDockCard *card);
};

#endif // __CDOCKSIDEBAR_H__