#ifndef HOMEWINDOWPRIVATE_H
#define HOMEWINDOWPRIVATE_H

#include "Windows/private/PlainWindow_p.h"

#include "HomeWindow.h"

#include "CNavFrame.h"
#include "CTabButton.h"

#include <QSplitter>
#include <QBoxLayout>

class HomeWindowPrivate : public PlainWindowPrivate {
    Q_DECLARE_PUBLIC(HomeWindow)
public:
    HomeWindowPrivate();
    ~HomeWindowPrivate();

    void init();

    CNavFrame *frame;

    QAbstractButton *recentButton;
    QAbstractButton *recoverButton;
    CTabButton *aboutButton;

    QWidget *recentWidget;
    QWidget *recoveryWidget;
};

#endif // HOMEWINDOWPRIVATE_H
