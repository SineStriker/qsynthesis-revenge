#ifndef HOMEWINDOWPRIVATE_H
#define HOMEWINDOWPRIVATE_H

#include "HomeWindow.h"

#include <QBoxLayout>
#include <QWidget>

#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

class HomeWindowPrivate {
    Q_DECLARE_PUBLIC(HomeWindow)
public:
    HomeWindowPrivate();
    virtual ~HomeWindowPrivate();

    void init();

    void reloadStrings_helper();

    HomeWindow *q_ptr;

    QWidget *w;

    QVBoxLayout *mainLayout;
    QHBoxLayout *topLayout;

    QLineEdit *searchBox;

    QPushButton *newButton;
    QPushButton *openButton;

    QListWidget *fileList;
};

#endif // HOMEWINDOWPRIVATE_H
