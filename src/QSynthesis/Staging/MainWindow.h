#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BasicWindow.h"

#include "CCoupleTabFrame.h"
#include "CentralTabWidget.h"

#include <QVBoxLayout>

class TabManager;
class ActionManager;

class MainWindow : public BasicWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void reloadStrings();

public:
    CentralTabWidget *tabWidget() const;
    CCoupleTabFrame *frame() const;

protected:
    QWidget *m_widget;
    QVBoxLayout *m_layout;

    CentralTabWidget *m_tabs;
    CCoupleTabFrame *m_frame;

    TabManager *m_tabMgr;
    ActionManager *m_actionMgr;
};
#endif // MAINWINDOW_H
