#ifndef HOMEWINDOWPRIVATE_H
#define HOMEWINDOWPRIVATE_H

#include <QBoxLayout>
#include <QSplitter>

#include "Windows/private/PlainWindow_p.h"

#include "HomeRecentWidget.h"
#include "HomeWindow.h"

#include "CNavFrame.h"

class HomeWindowPrivate : public PlainWindowPrivate {
    Q_DECLARE_PUBLIC(HomeWindow)
public:
    HomeWindowPrivate();
    ~HomeWindowPrivate();

    void init();
    void deinit();

    CNavFrame *frame;

    QAbstractButton *recentButton;
    QAbstractButton *recoverButton;
    CTabButton *aboutButton;

    HomeRecentWidget *recentWidget;
    QWidget *recoveryWidget;
};

#endif // HOMEWINDOWPRIVATE_H
