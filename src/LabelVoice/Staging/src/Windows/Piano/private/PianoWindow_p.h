#ifndef PIANOWINDOWPRIVATE_H
#define PIANOWINDOWPRIVATE_H

#include "Windows/Basic/ProjectWindow_p.h"

#include "CDockFrame.h"
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

    CDockFrame *frame;
    QScrollableTabWidget *tabs;

    QAbstractButton *expCard;
    QAbstractButton *spkCard;
    QAbstractButton *langCard;

    VExplorerPanel *expPanel;
    QWidget *spkPanel;
    QWidget *langPanel;

    PianoActions *actions;
};

#endif // PIANOWINDOWPRIVATE_H
