#include "WindowManager.h"
#include "private/WindowManager_p.h"

#include "Kernel/Events.h"
#include "Kernel/LvApplication.h"

#include "QMSystem.h"
#include "ViewHelper.h"

#include "Windows/Piano/PianoWindow.h"

#include <QEvent>
#include <QMessageBox>

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
    View::centralizeWindow(w, QSizeF(0.75, 0.75));

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
    w->setFilename(filename);
    if (!w->load()) {
        QMessageBox::critical(
            firstWindow(), qApp->errorTitle(),
            tr("Failed to load project %1").arg(QDir::toNativeSeparators(filename)));
        w->deleteLater();
        return nullptr;
    }

    w->show();
    View::centralizeWindow(w, QSizeF(0.75, 0.75));
    hideHome();
    d->projWins.insert(w);

    return w;
}

QMainWindow *WindowManager::firstWindow() const {
    Q_D(const WindowManager);
    if (d->homeWin) {
        return d->homeWin;
    }
    if (!d->projWins.isEmpty()) {
        return *d->projWins.begin();
    }
    return nullptr;
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

WindowManager::WindowManager(WindowManagerPrivate &d, QObject *parent)
    : QsAbstractManager(d, parent) {
    construct();

    d.init();
}

bool WindowManager::WindowManager::eventFilter(QObject *obj, QEvent *event) {
    Q_D(WindowManager);
    ProjectWindow *w;
    if ((w = qobject_cast<ProjectWindow *>(obj)) != nullptr) {
        switch (static_cast<int>(event->type())) {
            case QEventImpl::WindowClose: {
                // Detach Window
                // qDebug() << "?";
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
    return QsAbstractManager::eventFilter(obj, event);
}
