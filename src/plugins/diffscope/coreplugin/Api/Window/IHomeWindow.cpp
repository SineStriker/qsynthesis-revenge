#include "IHomeWindow.h"

namespace Core {

    IHomeWindow::IHomeWindow(QObject *parent) : ICoreWindow(HOME_WINDOW_ID, parent) {
    }

    IHomeWindow::~IHomeWindow() {
    }

}
