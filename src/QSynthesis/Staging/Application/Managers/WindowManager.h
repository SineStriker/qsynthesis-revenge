#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "BaseManager.h"
#include "Macros.h"

#define qWindows WindowManager::instance()

class MainWindow;
class WindowManagerPrivate;

class WindowManager : public BaseManager {
    Q_OBJECT
    Q_SINGLETON(WindowManager)
    Q_DECLARE_PRIVATE(WindowManager)
public:
    explicit WindowManager(QObject *parent = nullptr);
    ~WindowManager();

public:
    MainWindow *createWindow();

protected:
    WindowManager(WindowManagerPrivate &d, QObject *parent = nullptr);

    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // WINDOWMANAGER_H
