#ifndef HOMEWINDOWPRIVATE_H
#define HOMEWINDOWPRIVATE_H

#include "Controls/Windows/PlainWindow_p.h"

#include "../HomeWindow.h"

#include <QBoxLayout>
#include <QWidget>

#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

class HomeWindowPrivate : public PlainWindowPrivate {
    Q_DECLARE_PUBLIC(HomeWindow)
public:
    HomeWindowPrivate();
    virtual ~HomeWindowPrivate();

    void init();

    void reloadStrings_helper();

    QWidget *w;

    QVBoxLayout *mainLayout;
    QHBoxLayout *topLayout;

    QLineEdit *searchBox;

    QPushButton *newButton;
    QPushButton *openButton;

    QListWidget *fileList;

    QWidgetWindow *winHandle;
};

#endif // HOMEWINDOWPRIVATE_H
