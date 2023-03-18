#include "IProjectWindow.h"

namespace Core {

    IProjectWindow::IProjectWindow(QObject *parent) : ICoreWindow(PROJECT_WINDOW_ID, parent) {
    }

    IProjectWindow::~IProjectWindow() {
    }

}
