#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

/*
 * Usage: Manage windows
 *
 *
 */

#include "qsutils_macros.h"
#include "Vendor/Objects/BasicManager.h"

class HomeWindow;
class PianoWindow;
class WindowManagerPrivate;

#define qWindows WindowManager::instance()

class WindowManager : public BasicManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(WindowManager)

    Q_SINGLETON(WindowManager)
public:
    WindowManager(QObject *parent = nullptr);
    ~WindowManager();

public:
    HomeWindow *showHome();
    void hideHome();

    PianoWindow *newProject();
    PianoWindow *openProject(const QString &filename);

    bool exit();

protected:
    WindowManager(WindowManagerPrivate &d, QObject *parent = nullptr);

    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // WINDOWMANAGER_H
