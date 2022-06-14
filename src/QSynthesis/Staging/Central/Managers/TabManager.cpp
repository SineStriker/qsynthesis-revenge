#include "TabManager.h"
#include "TabManager_p.h"

#include "DocumentTab.h"
#include "MainWindow.h"

#include "DataManager.h"
#include "SystemHelper.h"

#include "CMenu.h"

#include <QApplication>
#include <QEvent>

TabManager::TabManager(MainWindow *parent) : TabManager(*new TabManagerPrivate(), parent) {
}

TabManager::~TabManager() {
}

bool TabManager::load() {
    Q_D(TabManager);

    auto tabs = d->w->tabWidget();

    auto tab1 = new DocumentTab();
    tab1->setFilename("1.ust");

    auto tab2 = new DocumentTab();
    tab2->setFilename("888.ust");

    tabs->addTab(tab1, QString());
    tabs->addTab(tab2, QString());

    return true;
}

TabManager::TabManager(TabManagerPrivate &d, QObject *parent) : BaseManager(d, parent) {
    d.init();
}

bool TabManager::eventFilter(QObject *obj, QEvent *event) {
    Q_D(TabManager);
    if (obj == d->w) {
        switch (event->type()) {
        // Proxy Drag And Drop Events To Tabs
        case QEvent::DragEnter:
        case QEvent::Drop: {
            auto tabs = d->w->tabWidget();
            tabs->event(event);
            if (event->isAccepted()) {
                return true;
            }
            break;
        }
        default:
            break;
        }
    }
    return BaseManager::eventFilter(obj, event);
}

void TabManager::_q_tabCloseRequested(int index) {
    Q_D(TabManager);
    d->tryCloseTab(index);
}

void TabManager::_q_tabIndexChanged(int index, int orgIndex) {
}

void TabManager::_q_tabTitleChanged(const QString &title) {
    Q_D(TabManager);
    d->w->setWindowTitle(QString("%1 - %2").arg(title, qData->windowTitle()));
}

void TabManager::_q_tabBarClicked(Qt::MouseButton button, int index) {
    Q_D(TabManager);
    if (button == Qt::RightButton) {
        auto tab = d->tabAt(index);
        if (!tab) {
            return;
        }

        CMenu menu(d->w);

        QAction closeAction(tr("Close"), &menu);
        QAction closeOthersAction(tr("Close Others"), &menu);
        QAction closeSavedAction(tr("Close Saved"), &menu);
        QAction closeAllAction(tr("Close All"), &menu);
        QAction newWinAction(tr("Open in new window(&E)"), &menu);
        QAction revealAction(&menu);

        menu.addAction(&closeAction);
        menu.addAction(&closeOthersAction);
        menu.addAction(&closeSavedAction);
        menu.addAction(&closeAllAction);

        if (d->tabCount() > 1) {
            menu.addSeparator();
            menu.addAction(&newWinAction);
        }
        if ((tab->type() & CentralTab::Document) && Sys::isFileExist(tab->filename())) {
            revealAction.setText(tr("Show in %1(&S)").arg(qData->fileManagerName()));
            menu.addSeparator();
            menu.addAction(&revealAction);
        } else if ((tab->type() & CentralTab::Folder) && Sys::isDirExist(tab->filename())) {
            revealAction.setText(tr("Open in %1(&S)").arg(qData->fileManagerName()));
            menu.addSeparator();
            menu.addAction(&revealAction);
        }

        QAction *action = menu.exec(QCursor::pos());
        d->w->tabWidget()->invalidateHover();

        if (action == &closeAction) {
            d->tryCloseTab(index);
        } else if (action == &closeOthersAction) {
            for (int i = d->tabCount() - 1; i >= 0; --i) {
                if (i == index) {
                    continue;
                }
                if (!d->tryCloseTab(i)) {
                    break;
                }
            }
        } else if (action == &closeSavedAction) {
            for (int i = d->tabCount() - 1; i >= 0; --i) {
                if (d->tabAt(i)->isEdited()) {
                    continue;
                }
                if (!d->tryCloseTab(i)) {
                    break;
                }
            }
        } else if (action == &closeAllAction) {
            for (int i = d->tabCount() - 1; i >= 0; --i) {
                if (!d->tryCloseTab(i)) {
                    break;
                }
            }
        } else if (action == &newWinAction) {
            //            CentralTab *tab = tabAt(index);
            //            tabs->removeTab(index);
            //            MainWindow *w = new MainWindow({tab});
            //            if (!isMaximized()) {
            //                w->resize(size());
            //            } else {
            //                w->resize(QApplication::desktop()->size() * 2.0 / 3.0);
            //            }
            //            w->centralize();
            //            w->showForward();
        } else if (action == &revealAction) {
            Sys::reveal(d->tabAt(index)->filename());
        }
    }
}
