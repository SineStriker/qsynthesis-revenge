#include "ProjectWindowAddOn.h"

#include <QDebug>
#include <QLabel>
#include <QMenuBar>

#include "CoreApi/IWindow.h"

namespace Core {

    namespace Internal {

        bool ProjectWindowAddOnFactory::predicate(IWindow *iWindow) const {
            return iWindow->id() == "project";
        }

        IWindowAddOn *ProjectWindowAddOnFactory::create(QObject *parent) {
            return new ProjectWindowAddOn(parent);
        }

        ProjectWindowAddOn::ProjectWindowAddOn(QObject *parent) : IWindowAddOn(parent) {
        }

        ProjectWindowAddOn::~ProjectWindowAddOn() {
        }

        void ProjectWindowAddOn::initialize() {
            auto iWin = this->windowHandle();
            auto win = iWin->window();

            auto label = new QLabel("123");
            iWin->setCentralWidget(label);
            iWin->menuBar()->addMenu(new QMenu("File(&F)"));
        }

        void ProjectWindowAddOn::extensionsInitialized() {
        }
    }

}