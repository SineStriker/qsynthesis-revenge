#ifndef __CDOCKSIDEBAR_H__
#define __CDOCKSIDEBAR_H__

#include <QFrame>

#include "CDockTabBar.h"
#include "QMWidgetToolsGlobal.h"

class CDockFrame;

class CDockTabDragProxy;

class QMWTOOLS_EXPORT CDockSideBar : public QFrame {
    Q_OBJECT
public:
    explicit CDockSideBar(QWidget *parent = nullptr);
    ~CDockSideBar();

    friend class CDockTabBar;

private:
    void init();

public:
    QM::Direction cardDirection() const;
    void setCardDirection(QM::Direction cardDirection);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orient);

    Qt::Edge edge() const;

    bool highlight() const;
    void setHighlight(bool highlight, int widthHint = 0);

    int count() const;

    CDockTabBar *firstBar() const;
    CDockTabBar *secondBar() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    CDockFrame *frame() const;

protected:
    CDockTabBar *m_firstBar;
    CDockTabBar *m_secondBar;

    void resetLayout();

    void mousePressEvent(QMouseEvent *event) override;

private:
    void _q_cardAdded(CDockCard *card);
    void _q_cardRemoved(CDockCard *card);
    void _q_cardToggled(CDockCard *card);
    void _q_cardViewModeChanged(CDockCard *card, CDockCard::ViewMode oldViewMode);

signals:
    void cardAdded(QM::Priority number, CDockCard *card);
    void cardRemoved(QM::Priority number, CDockCard *card);
    void cardToggled(QM::Priority number, CDockCard *card);
    void cardViewModeChanged(QM::Priority number, CDockCard *card, CDockCard::ViewMode oldViewMode);
};

#endif // __CDOCKSIDEBAR_H__