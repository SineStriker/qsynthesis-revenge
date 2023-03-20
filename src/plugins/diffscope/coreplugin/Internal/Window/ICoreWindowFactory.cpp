#include "ICoreWindowFactory.h"

#include "coreplugin/Api/Home/IHomeWindow.h"
#include "coreplugin/Api/Project/IProjectWindow.h"

namespace Core {

    namespace Internal {

        IHomeWindowFactory::IHomeWindowFactory() : IWindowFactory(IHomeWindow::WindowTypeID(), Create) {
        }

        IHomeWindowFactory::~IHomeWindowFactory() {
        }

        IWindow *IHomeWindowFactory::create(QObject *parent) {
            return new IHomeWindow(parent);
        }

        IProjectWindowFactory::IProjectWindowFactory() : IWindowFactory(IProjectWindow::WindowTypeID(), Create) {
        }

        IProjectWindowFactory::~IProjectWindowFactory() {
        }

        IWindow *IProjectWindowFactory::create(QObject *parent) {
            return new IProjectWindow(parent);
        }
    }
}