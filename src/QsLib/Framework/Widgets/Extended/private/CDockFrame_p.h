#ifndef __CDOCKFRAME_P_H__
#define __CDOCKFRAME_P_H__

#include <QGridLayout>
#include <QHash>
#include <QObject>

#include "../CDockFrame.h"
#include "QsNamespace.h"

#include "CDockPanel.h"
#include "CDockPanelSplitter.h"
#include "CDockSideBar.h"
#include "CDockTabDragProxy.h"

class CDockFramePrivate : public QObject {
    Q_OBJECT
public:
    CDockFramePrivate(CDockFrame *parent);
    ~CDockFramePrivate();

    void init();

    CDockFrame *q;

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

    QSize m_dragAreaSize;

private:
    void _q_cardAdded(Qs::Priority number, CDockCard *card);
    void _q_cardRemoved(Qs::Priority number, CDockCard *card);
    void _q_cardToggled(Qs::Priority number, CDockCard *card);
};

#endif // __CDOCKFRAME_P_H__
