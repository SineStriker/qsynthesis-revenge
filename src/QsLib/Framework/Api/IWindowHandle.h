#ifndef IWINDOWHANDLE_H
#define IWINDOWHANDLE_H

#include <QMainWindow>

#include "QsFrameworkGlobal.h"

class IWindowHandlePrivate;

class QSFRAMEWORK_API IWindowHandle : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(IWindowHandle)
public:
    IWindowHandle(QMainWindow *parent);
    ~IWindowHandle();
    
    void setParent(QObject *) = delete;

public:
    /**
     * @brief Parent window, cast from QWidget::parentWidget
     *
     */
    QMainWindow *window() const;

    /**
     * @brief Initialize the window handle, must call to take effect
     *
     */
    virtual void setup();

    /**
     * @brief Set the window menu bar, default to QMainWindow::setMenuBar
     *
     */
    virtual void setMenuBar(QMenuBar *menuBar);

    /**
     * @brief Current window menu bar, default to QMainWindow::menuBar
     *
     * @return QMenuBar*
     */
    virtual QMenuBar *menuBar() const;

    enum TitleBarFlag {
        WindowIcon = 0x1,
        WindowTitle = 0x2,
        WindowMinimizeButton = 0x4,
        WindowMaximizeButton = 0x8,
        WindowCloseButton = 0x10,
        TitleBarFlagMask = WindowIcon | WindowTitle |                    //
                           WindowMinimizeButton | WindowMaximizeButton | //
                           WindowCloseButton,
    };
    Q_DECLARE_FLAGS(TitleBarFlags, TitleBarFlag);

    /**
     * @brief Similar to QWidget::setWindowFlags
     *
     */
    void setTitleBarFlags(TitleBarFlags flags);

    /**
     * @brief Similar to QWidget::windowFlags
     * 
     */
    TitleBarFlags titleBarFlags();

protected:
    IWindowHandle(IWindowHandlePrivate &d, QMainWindow *parent);

    QScopedPointer<IWindowHandlePrivate> d_ptr;
};

#endif // IWINDOWHANDLE_H
