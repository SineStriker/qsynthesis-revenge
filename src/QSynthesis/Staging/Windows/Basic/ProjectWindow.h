#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include "Controls/Windows/PlainWindow.h"

class ProjectWindowPrivate;

class ProjectWindow : public PlainWindow {
    Q_OBJECT
    Q_DECLARE_PRIVATE(ProjectWindow)
public:
    ProjectWindow(QWidget *parent = nullptr);
    ~ProjectWindow();

protected:
    ProjectWindow(ProjectWindowPrivate &d, QWidget *parent = nullptr);

    bool event(QEvent *event) override;

    void closeEvent(QCloseEvent *event) override;
};

#endif // PROJECTWINDOW_H
