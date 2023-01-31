#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "Windows/PlainWindow.h"

class HomeWindowPrivate;

class HomeWindow : public PlainWindow {
    Q_OBJECT
    Q_DECLARE_PRIVATE(HomeWindow)
public:
    HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

protected:
    HomeWindow(HomeWindowPrivate &d, QWidget *parent = nullptr);
};

#endif // HOMEWINDOW_H
