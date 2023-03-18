#include "ICoreWindowFactory.h"

#include "Window/IHomeWindow.h"
#include "Window/IProjectWindow.h"

namespace Core {

    namespace Internal {

        IHomeWindowFactory::IHomeWindowFactory() : IWindowFactory(HOME_WINDOW_ID, Create) {
        }

        IHomeWindowFactory::~IHomeWindowFactory() {
        }

        IWindow *IHomeWindowFactory::create(QObject *parent) {
            return new IHomeWindow(parent);
        }

        IProjectWindowFactory::IProjectWindowFactory() : IWindowFactory(PROJECT_WINDOW_ID, Create) {
        }

        IProjectWindowFactory::~IProjectWindowFactory() {
        }

        IWindow *IProjectWindowFactory::create(QObject *parent) {
            return new IProjectWindow(parent);
        }
    }
}