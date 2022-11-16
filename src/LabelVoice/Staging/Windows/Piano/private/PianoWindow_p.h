#ifndef PIANOWINDOWPRIVATE_H
#define PIANOWINDOWPRIVATE_H

#include "Windows/Basic/ProjectWindow_p.h"

#include "CCoupleTabFrame.h"
#include "QScrollableTabWidget.h"

#include "../PianoActions.h"
#include "../PianoWindow.h"

#include "../Panels/VExplorerPanel.h"

class PianoWindowPrivate : public ProjectWindowPrivate {
    Q_DECLARE_PUBLIC(PianoWindow)
public:
    PianoWindowPrivate();
    virtual ~PianoWindowPrivate();

    void init();

    void reloadStrings_helper();

    QString filename;

    CCoupleTabFrame *frame;
    QScrollableTabWidget *tabs;

    CCoupleTabBarCard *expCard;
    CCoupleTabBarCard *spkCard;
    CCoupleTabBarCard *langCard;

    VExplorerPanel *expPanel;
    QWidget *spkPanel;
    QWidget *langPanel;

    PianoActions *actions;
};

#endif // PIANOWINDOWPRIVATE_H
