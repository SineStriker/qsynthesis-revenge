#ifndef NATIVEWINDOW_H
#define NATIVEWINDOW_H

#include "CWindowBarV2.h"

#ifndef Q_OS_MAC
#include <FramelessMainWindow>
class NativeWindow : public FRAMELESSHELPER_NAMESPACE::FramelessMainWindow {
    Q_OBJECT
    using Super = FRAMELESSHELPER_NAMESPACE::FramelessMainWindow;
#else
#include <QMainWindow>

#include "CMenu.h"

class NativeWindow : public QMainWindow {
    Q_OBJECT
    using Super = QMainWindow;
#endif
public:
    explicit NativeWindow(QWidget *parent = nullptr);
    ~NativeWindow();

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

#endif // NATIVEWINDOW_H
