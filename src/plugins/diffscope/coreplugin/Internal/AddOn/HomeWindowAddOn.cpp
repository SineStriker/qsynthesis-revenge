#include "HomeWindowAddOn.h"

#include <QDebug>
#include <QLabel>
#include <QMenuBar>

#include "Window/IHomeWindow.h"

#include "ICore.h"

namespace Core {

    namespace Internal {

        bool HomeWindowAddOnFactory::predicate(IWindow *iWindow) const {
            return iWindow->id() == "home";
        }

        IWindowAddOn *HomeWindowAddOnFactory::create(QObject *parent) {
            return new HomeWindowAddOn(parent);
        }

        HomeWindowAddOn::HomeWindowAddOn(QObject *parent) : IWindowAddOn(parent) {
        }

        HomeWindowAddOn::~HomeWindowAddOn() {
        }

        void HomeWindowAddOn::initialize() {
            auto iWin = qobject_cast<IHomeWindow *>(this->handle());

            auto btn = iWin->addNavWidget(new QLabel("22222222222222222"));
            btn->setText("123456");

            reloadMenuBar();
        }

        void HomeWindowAddOn::extensionsInitialized() {
        }

        void HomeWindowAddOn::reloadMenuBar() {
            auto fileItem = new ActionItem("home_File", new QMenu("File(&F)"));
            auto editItem = new ActionItem("home_Edit", new QMenu("Edit(&E)"));
            auto helpItem = new ActionItem("home_Help", new QMenu("Help(&H)"));

            auto openGroupItem = new ActionItem("home_OpenGroup", new QActionGroup(this));

            auto newFileItem = new ActionItem("home_NewFile", new QAction("New File"));
            auto openFileItem = new ActionItem("home_OpenFile", new QAction("Open File"));

            ICore::instance()
                ->actionSystem()
                ->context("home_MainMenu")
                ->buildMenuBarWithState({fileItem, editItem, helpItem, openGroupItem, newFileItem, openFileItem},
                                        handle()->menuBar());
        }
    }

}