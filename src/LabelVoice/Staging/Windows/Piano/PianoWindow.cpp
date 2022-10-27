#include "PianoWindow.h"
#include "private/PianoWindow_p.h"

#include "Kernel/Events.h"
#include "Kernel/LvApplication.h"
#include "Managers/FileManager.h"
#include "Managers/WindowManager.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>

PianoWindow::PianoWindow(QWidget *parent) : PianoWindow(*new PianoWindowPrivate(), parent) {
}

PianoWindow::~PianoWindow() {
}

void PianoWindow::reloadStrings() {
    Q_D(PianoWindow);
    d->reloadStrings_helper();
}

PianoWindow::PianoWindow(PianoWindowPrivate &d, QWidget *parent)
    : ProjectWindow(d, parent), ProjectCommonBlock(this) {
    d.q_ptr = this;

    d.init();

    Q_TR_NOTIFY(PianoWindow)
}

void PianoWindow::closeEvent(QCloseEvent *event) {
    ProjectWindow::closeEvent(event);
}

void PianoWindow::_q_actionTriggered(int actionId) {
    Q_D(PianoWindow);
    switch (actionId) {
        case ActionImpl::File_New: {
            newProject();
            break;
        }
        case ActionImpl::File_Open: {
            openProject();
            break;
        }
        case ActionImpl::File_Close: {
            d->closeFlag = true;
            close();
            break;
        }
        case ActionImpl::File_Exit: {
            WindowManager::instance()->exit();
            break;
        }
        case ActionImpl::Help_AboutApplication: {
            aboutApp();
            break;
        }
        case ActionImpl::Help_AboutQt:
            aboutQt();
            break;
        default:
            break;
    }
}
