#include "ICoreWindowAddOn.h"

#include <QDebug>
#include <QLabel>
#include <QMenuBar>

#include "Window/IWindow.h"

namespace Core {

    namespace Internal {

        bool ICoreWindowAddOnFactory::predicate(IWindow *iWindow) const {
            return iWindow->id() == "home" || iWindow->id() == "project";
        }

        IWindowAddOn *ICoreWindowAddOnFactory::create(QObject *parent) {
            return new ICoreWindowAddOn(parent);
        }

        ICoreWindowAddOn::ICoreWindowAddOn(QObject *parent) : IWindowAddOn(parent) {
        }

        ICoreWindowAddOn::~ICoreWindowAddOn() {
        }

        void ICoreWindowAddOn::initialize() {
            auto iWin = this->handle();
            auto win = iWin->window();

            auto label = new QLabel("123");
            iWin->setCentralWidget(label);
            iWin->menuBar()->addMenu(new QMenu("File(&F)"));
        }

        void ICoreWindowAddOn::extensionsInitialized() {
        }
    }

}