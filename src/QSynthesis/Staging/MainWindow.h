#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BasicWindow.h"

class MainWindow : public BasicWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void reloadStrings();
};
#endif // MAINWINDOW_H
