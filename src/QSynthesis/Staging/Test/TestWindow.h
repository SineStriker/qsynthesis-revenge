#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include "BasicWindow.h"

class TestWindow : public BasicWindow {
    Q_OBJECT
public:
    explicit TestWindow(QWidget *parent = nullptr);
    ~TestWindow();

    void reloadStrings();

signals:
};

#endif // TESTWINDOW_H
