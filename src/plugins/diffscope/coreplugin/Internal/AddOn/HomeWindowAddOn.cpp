#include "HomeWindowAddOn.h"

#include <QDebug>
#include <QLabel>
#include <QMenuBar>

#include "Window/IHomeWindow.h"

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
            iWin->menuBar()->addMenu(new QMenu("File(&F)"));

            auto btn = iWin->addNavWidget(new QWidget());
            btn->setText("123456");
        }

        void HomeWindowAddOn::extensionsInitialized() {
        }
    }

}