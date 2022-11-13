#include "PianoWindow.h"
#include "private/PianoWindow_p.h"

#include "Kernel/Events.h"
#include "Kernel/LvApplication.h"
#include "Managers/FileManager.h"
#include "Managers/WindowManager.h"

#include "Serialization/QLVProject.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>

PianoWindow::PianoWindow(QWidget *parent) : PianoWindow(*new PianoWindowPrivate(), parent) {
}

PianoWindow::~PianoWindow() {
}

QString PianoWindow::filename() const {
    Q_D(const PianoWindow);
    return d->filename;
}

void PianoWindow::setFilename(const QString &filename) {
    Q_D(PianoWindow);
    d->filename = filename;
}

bool PianoWindow::load() {
    Q_D(PianoWindow);
    LVModel::ProjectModel proj;

    if (!proj.load(d->filename)) {
        FileManager::instance()->commitRecent(FileManager::Project, FileManager::Remove,
                                              d->filename);
        return false;
    }
    FileManager::instance()->commitRecent(FileManager::Project, FileManager::Advance, d->filename);

    // Update View
    {
        QList<PianoSpec::SpeakerDesc> speakers;
        for (const auto &spk : qAsConst(proj.Speakers)) {
            speakers.append(PianoSpec::SpeakerDesc{spk.Id, spk.Name});
        }
        d->expPanel->setSpeakers(speakers);

        QList<QPair<QString, PianoSpec::ItemDesc>> items;
        for (const auto &item : qAsConst(proj.ItemResources)) {
            items.append(qMakePair(item.VirtualPath, //
                                   PianoSpec::ItemDesc{
                                       item.Id,
                                       item.Name,
                                       item.Speaker,
                                       item.Type == LVModel::ItemResource::Placeholder,
                                   }));
        }
        d->expPanel->setItems(items);
    }

    return true;
}

bool PianoWindow::save() {
    return true;
}

void PianoWindow::reloadStrings() {
    Q_D(PianoWindow);
    d->reloadStrings_helper();
}

PianoWindow::PianoWindow(PianoWindowPrivate &d, QWidget *parent)
    : ProjectWindow(d, parent), ProjectCommonBlock(this) {
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
