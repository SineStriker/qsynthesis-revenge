#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "Windows/PlainWindow.h"

class HomeWindowPrivate;

class HomeWindow : public PlainWindow {
    Q_OBJECT
    Q_DECLARE_PRIVATE(HomeWindow)
public:
    explicit HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

    void reloadStrings();

protected:
    HomeWindow(HomeWindowPrivate &d, QWidget *parent = nullptr);

private:
    void _q_aboutButtonClicked();
};

#endif // HOMEWINDOW_H
