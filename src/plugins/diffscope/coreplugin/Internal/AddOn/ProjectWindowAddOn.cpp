#include "ProjectWindowAddOn.h"

#include <QDebug>
#include <QLabel>
#include <QMenuBar>

#include <QEvent>
#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include "CoreApi/IWindow.h"
#include "ICore.h"
#include "Window/IProjectWindow.h"

#include "Internal/Utils/LastWindowFilter.h"

namespace Core {

    namespace Internal {

        bool ProjectWindowAddOnFactory::predicate(IWindow *handle) const {
            return handle->id() == "project";
        }

        IWindowAddOn *ProjectWindowAddOnFactory::create(QObject *parent) {
            return new ProjectWindowAddOn(parent);
        }

        ProjectWindowAddOn::ProjectWindowAddOn(QObject *parent) : CoreWindowAddOn(parent) {
        }

        ProjectWindowAddOn::~ProjectWindowAddOn() {
        }

        void ProjectWindowAddOn::initialize() {
            CoreWindowAddOn::initialize();

            initActions();

            qIDec->installLocale(this, _LOC(ProjectWindowAddOn, this));
        }

        void ProjectWindowAddOn::extensionsInitialized() {
            CoreWindowAddOn::extensionsInitialized();
        }

        void ProjectWindowAddOn::reloadStrings() {
            saveGroupItem->setText(tr("Save Actions"));
            saveFileItem->setText(tr("&Save"));
            saveAsFileItem->setText(tr("Save &As..."));

            exitGroupItem->setText(tr("Exit Actions"));
            closeFileItem->setText(tr("Close"));

            welcomeGroupItem->setText(tr("Welcome Actions"));
            showHomeItem->setText(tr("Show Home"));
        }

        void ProjectWindowAddOn::initActions() {
            auto iWin = windowHandle()->cast<IProjectWindow>();
            auto win = iWin->window();

            saveGroupItem = new ActionItem("core.SaveGroup", new QActionGroup(this), this);
            saveFileItem = new ActionItem("core.SaveFile", new QAction(this), this);
            saveAsFileItem = new ActionItem("core.SaveAsFile", new QAction(this), this);

            exitGroupItem = new ActionItem("core.ExitGroup", new QActionGroup(this), this);
            closeFileItem = new ActionItem("core.CloseFile", new QAction(this), this);

            welcomeGroupItem = new ActionItem("core.WelcomeGroup", new QActionGroup(this), this);
            showHomeItem = new ActionItem("core.ShowHome", new QAction(this), this);

            connect(iWin->doc(), &IDocument::changed, this,
                    [this, iWin]() { saveFileItem->setEnabled(iWin->doc()->isModified()); });

            connect(saveAsFileItem->action(), &QAction::triggered, this, [this, iWin]() {
                auto doc = iWin->doc();
                auto filePath = doc->filePath();
                if (!QMFs::isFileExist(filePath)) {
                    saveAsFileItem->action()->trigger();
                } else {
                    ICore::instance()->documentSystem()->saveDocument(iWin->doc());
                }
            });

            connect(saveAsFileItem->action(), &QAction::triggered, this, [this, iWin]() {
                IWindowContext ctx(iWin);
                ICore::instance()->documentSystem()->saveFileBrowse(&ctx, iWin->doc());
            });

            connect(closeFileItem->action(), &QAction::triggered, this, [this, win]() {
                LastWindowFilter guard;
                win->close();
            });

            connect(showHomeItem->action(), &QAction::triggered, this, [this]() {
                ICore::showHome(); //
            });

            iWin->addActionItems({
                saveGroupItem,
                saveFileItem,
                saveAsFileItem,
                exitGroupItem,
                closeFileItem,
                welcomeGroupItem,
                showHomeItem,
            });
        }
    }

}