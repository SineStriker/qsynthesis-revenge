#ifndef BASICWINDOW_H
#define BASICWINDOW_H

#include <FramelessMainWindow>

#include "CWindowBarV2.h"

class BasicWindow : public FRAMELESSHELPER_PREPEND_NAMESPACE(FramelessMainWindow) {
    Q_OBJECT
public:
    explicit BasicWindow(QWidget *parent = nullptr);
    ~BasicWindow();

public:
    void setMenuBar(QMenuBar *menuBar);
    QMenuBar *menuBar() const;

    void centralize(double r);

protected:
    CWindowBarV2 *m_titleBar;

signals:
};

#endif // BASICWINDOW_H
