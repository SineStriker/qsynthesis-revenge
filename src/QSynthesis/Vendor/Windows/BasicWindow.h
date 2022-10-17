#ifndef BASICWINDOW_H
#define BASICWINDOW_H

#include "CWindowBarV2.h"

#ifndef Q_OS_MAC
#include <FramelessMainWindow>
class BasicWindow : public FRAMELESSHELPER_NAMESPACE::FramelessMainWindow {
    Q_OBJECT
    using Super = FRAMELESSHELPER_NAMESPACE::FramelessMainWindow;
#else
// #include "MacMainWindow.h"
// class BasicWindow : public MacMainWindow {
//     Q_OBJECT
//     using Super = MacMainWindow;

#include <QMainWindow>

#include "CMenu.h"

class BasicWindow : public QMainWindow {
    Q_OBJECT
    using Super = QMainWindow;
#endif
public:
    explicit BasicWindow(QWidget *parent = nullptr);
    ~BasicWindow();

public:

#ifndef Q_OS_MAC
    void setMenuBar(QMenuBar *menuBar);
    QMenuBar *menuBar() const;
#endif

    void resizeByDesktop(double r, bool centralize = false);

protected:

#ifndef Q_OS_MAC
    CWindowBarV2 *m_titleBar;
#endif

    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
};

#endif // BASICWINDOW_H
