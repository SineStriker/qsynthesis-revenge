#include "FilesActionEngine.h"
#include "FilesActionEngine_p.h"

#include <QDebug>
#include <QMessageBox>

#include "DataManager.h"
#include "FileParser.h"

static const char FLAG_OPEN[] = "%PROJ%";
static const char FLAG_OPEN_FOLDER[] = "%DIR%";
static const char FLAG_SAVE[] = "%SAVE%";
static const char FLAG_IMPORT[] = "%IMPORT%";
static const char FLAG_APPEND[] = "%APPEND%";

FilesActionEngine::FilesActionEngine(QObject *parent)
    : FilesActionEngine(*new FilesActionEnginePrivate(), parent) {
}

FilesActionEngine::~FilesActionEngine() {
}

FilesActionEngine::FilesActionEngine(FilesActionEnginePrivate &d, QObject *parent)
    : ActionManagerEngine(d, parent) {
    d.init();
}

void FilesActionEngine::q_actionTriggered(QAction *action) {
    auto menu = qobject_cast<QMenu *>(sender());

    Q_D(FilesActionEngine);
    if (menu == d->fileMenu) {
        if (action == d->file_importFile) {
            QString path =
                qData->openFile(tr("Import"), qData->getFileFilter(DataManager::ImportFile),
                                FLAG_IMPORT, d->window());
            if (!path.isEmpty()) {
                d->eventMgr->import(path);
            }
        }
    } else if (menu == d->helpMenu) {
        if (action == d->help_aboutApp) {
        } else if (action == d->help_aboutQt) {
            QMessageBox::aboutQt(d->window(), tr("About %1").arg("Qt"));
        }
    }
}
