#include "WindowManager.h"
#include "private/WindowManager_p.h"

#include "Kernel/Events.h"

#include "SystemHelper.h"
#include "ViewHelper.h"

#include <QEvent>

Q_SINGLETON_DECLARE(WindowManager);

WindowManager::WindowManager(QObject *parent) : WindowManager(*new WindowManagerPrivate(), parent) {
}

WindowManager::~WindowManager() {
}

HomeWindow *WindowManager::showHome() {
    Q_D(WindowManager);

    if (d->homeWin) {
        return d->homeWin;
    }

    auto w = d->createHomeWin();
    w->show();
    View::centralizeWindow(w, QSizeF(0.4, 0.6));

    d->homeWin = w;

    return w;
}

void WindowManager::hideHome() {
    Q_D(WindowManager);

    if (!d->homeWin) {
        return;
    }

    d->homeWin->close();
    d->homeWin = nullptr;
}

PianoWindow *WindowManager::newProject() {
    Q_D(WindowManager);

    auto w = d->createPianoWin();
    w->show();
    View::centralizeWindow(w, QSizeF(0.75, 0.75));

    hideHome();

    d->projWins.insert(w);
    return w;
}

PianoWindow *WindowManager::openProject(const QString &filename) {
    if (!Sys::isFileExist(filename)) {
        return nullptr;
    }

    Q_D(WindowManager);

    auto w = d->createPianoWin();
    w->show();
    View::centralizeWindow(w, QSizeF(0.75, 0.75));

    hideHome();

    d->projWins.insert(w);
    return w;
}

bool WindowManager::exit() {
    Q_D(WindowManager);

    bool ok = true;
    auto windows = d->projWins.values();
    for (auto it = windows.rbegin(); it != windows.rend(); ++it) {
        auto win = *it;
        win->close();

        if (win->isVisible()) {
            ok = false;
            break;
        }
    }

    return ok;
}

WindowManager::WindowManager(WindowManagerPrivate &d, QObject *parent) : BasicManager(d, parent) {
    construct();

    d.init();
}

bool WindowManager::WindowManager::eventFilter(QObject *obj, QEvent *event) {
    Q_D(WindowManager);
    if (qstrcmp(obj->metaObject()->className(), "PianoWindow") == 0) {
        auto w = qobject_cast<PianoWindow *>(obj);
        switch (static_cast<int>(event->type())) {
            case QEventImpl::WindowClose: {
                // Detach Window
                auto e = static_cast<QEventImpl::WindowCloseEvent *>(event);
                d->projWins.remove(w);
                if (e->closeOnly() && d->projWins.isEmpty()) {
                    showHome();
                }
                break;
            }
            default:
                break;
        }
    }
    return BasicManager::eventFilter(obj, event);
}
