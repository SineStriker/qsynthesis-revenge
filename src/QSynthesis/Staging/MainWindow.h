#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BasicWindow.h"

#include "CCoupleTabFrame.h"
#include "CentralTabWidget.h"

#include <QVBoxLayout>

class TabManager;
class EventManager;
class ActionManager;

class MainWindow : public BasicWindow {
    Q_OBJECT
public:
    enum EditType {
        Files,
        Folder,
    };

    MainWindow(EditType editType, QWidget *parent = nullptr);
    ~MainWindow();

    void reloadStrings();

public:
    CentralTabWidget *tabWidget() const;
    CCoupleTabFrame *frame() const;

    EditType editType() const;

    TabManager *tabMgr() const;
    EventManager *eventMgr() const;
    ActionManager *actionMgr() const;

protected:
    EditType m_editType;

    QWidget *m_widget;
    QVBoxLayout *m_layout;

    CentralTabWidget *m_tabs;
    CCoupleTabFrame *m_frame;

    TabManager *m_tabMgr;
    EventManager *m_eventMgr;
    ActionManager *m_actionMgr;
};
#endif // MAINWINDOW_H
