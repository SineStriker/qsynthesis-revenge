#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "Vendor/Windows/BasicWindow.h"

class HomeWindowPrivate;

class HomeWindow : public BasicWindow {
    Q_OBJECT
    Q_DECLARE_PRIVATE(HomeWindow)
public:
    HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

public:
    void reloadStrings();

protected:
    HomeWindow(HomeWindowPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<HomeWindowPrivate> d_ptr;

private:
    void _q_searchBoxChanged(const QString &text);

    void _q_newButtonClicked();
    void _q_openButtonClicked();
};

#endif // HOMEWINDOW_H
