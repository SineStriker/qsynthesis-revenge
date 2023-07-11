#ifndef __CDOCKFRAME_P_H__
#define __CDOCKFRAME_P_H__

#include <QGridLayout>
#include <QHash>
#include <QObject>

#include "../CDockFrame.h"
#include "QMNamespace.h"

#include "CDockPanel.h"
#include "CDockPanelSplitter.h"
#include "CDockSideBar.h"
#include "CDockTabDragProxy.h"

class CDockFramePrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(CDockFrame)
public:
    CDockFramePrivate();
    ~CDockFramePrivate();

    void init();

    QList<int> orgHSizes;
    QList<int> orgVSizes;

    CDockFrame *q_ptr;

    QGridLayout *m_layout;

    CDockSideBar *m_leftBar;
    CDockSideBar *m_topBar;
    CDockSideBar *m_rightBar;
    CDockSideBar *m_bottomBar;

    CDockPanel *m_leftPanel;
    CDockPanel *m_topPanel;
    CDockPanel *m_rightPanel;
    CDockPanel *m_bottomPanel;

    CDockPanelSplitter *m_verticalSplitter;
    CDockPanelSplitter *m_horizontalSplitter;

    QStackedWidget *m_centralContainer;

    CDockTabDragProxy *m_dragProxy;

private:
    void _q_cardAdded(QM::Priority number, CDockCard *card);
    void _q_cardRemoved(QM::Priority number, CDockCard *card);
    void _q_cardToggled(QM::Priority number, CDockCard *card);
    void _q_cardViewModeChanged(QM::Priority number, CDockCard *card, CDockCard::ViewMode oldViewMode);
};

#endif // __CDOCKFRAME_P_H__
