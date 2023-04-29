#include "ProjectWindowAddOn.h"

#include <QDebug>
#include <QLabel>
#include <QMenuBar>

#include <QMDecoratorV2.h>

#include "CoreApi/IWindow.h"
#include "ICore.h"

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
            welcomeGroupItem->setText(tr("Welcome Actions"));
            showHomeItem->setText(tr("Show Home"));
        }

        void ProjectWindowAddOn::initActions() {
            auto iWin = windowHandle();
            auto win = iWin->window();

            welcomeGroupItem = new ActionItem("core.WelcomeGroup", new QActionGroup(this), this);
            showHomeItem = new ActionItem("core.ShowHome", new QAction(), this);

            connect(showHomeItem->action(), &QAction::triggered, this, [this]() {
                ICore::showHome(); //
            });

            iWin->addActionItems({
                welcomeGroupItem,
                showHomeItem,
            });
        }
    }

}