#include "HomeWindowAddOn.h"

#include <QDebug>
#include <QLabel>
#include <QMenuBar>

#include "Window/IWindow.h"

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
            auto iWin = this->handle();
            auto win = iWin->window();

            auto label = new QLabel("123");
            iWin->setCentralWidget(label);
            iWin->menuBar()->addMenu(new QMenu("File(&F)"));
        }

        void HomeWindowAddOn::extensionsInitialized() {
        }
    }

}