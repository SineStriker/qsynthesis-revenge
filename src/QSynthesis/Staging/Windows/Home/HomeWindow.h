#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "Controls/Windows/PlainWindow.h"

class HomeWindowPrivate;

class HomeWindow : public PlainWindow {
    Q_OBJECT
    Q_DECLARE_PRIVATE(HomeWindow)
public:
    HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

public:
    void reloadStrings();

protected:
    HomeWindow(HomeWindowPrivate &d, QWidget *parent = nullptr);

private:
    void _q_searchBoxChanged(const QString &text);

    void _q_newButtonClicked();
    void _q_openButtonClicked();
};

#endif // HOMEWINDOW_H
