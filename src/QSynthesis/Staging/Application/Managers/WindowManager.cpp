#include "WindowManager.h"
#include "WindowManager_p.h"

#include "Logs/CRecordHolder.h"
#include "MainWindow.h"

#include <QEvent>
#include <QMessageBox>

Q_SINGLETON_DECLARE(WindowManager)

WindowManager::WindowManager(QObject *parent) : WindowManager(*new WindowManagerPrivate(), parent) {
}

WindowManager::~WindowManager() {
}

MainWindow *WindowManager::createWindow() {
    Q_D(WindowManager);

    auto w = new MainWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->installEventFilter(this);

    // Recover Window State
    QRect rect = qRecordCData.windowRect;
    bool max = qRecordCData.windowMaximized;
    if (!rect.isNull()) {
        w->setGeometry(rect);
    } else {
        w->centralize(2.0 / 3.0);
    }
    if (max) {
        w->showMaximized();
    }
    w->show();
    d->windows.insert(w);

    return w;
}

WindowManager::WindowManager(WindowManagerPrivate &d, QObject *parent) : BaseManager(d, parent) {
    construct();

    d.init();
}

bool WindowManager::eventFilter(QObject *obj, QEvent *event) {
    Q_D(WindowManager);
    if (strcmp(obj->metaObject()->className(), "MainWindow") == 0) {
        auto w = qobject_cast<MainWindow *>(obj);
        if (event->type() == QEvent::Close) {
            // Save Window State
            if (d->windows.size() == 1) {
                qRecordData.windowRect = w->geometry();
                qRecordData.windowMaximized = w->isMaximized();
            }
            d->windows.remove(w);
        }
    }
    return BaseManager::eventFilter(obj, event);
}
