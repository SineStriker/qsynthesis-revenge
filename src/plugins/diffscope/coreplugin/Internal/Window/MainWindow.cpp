#include "MainWindow.h"

#include <QMenuBar>

namespace Core {

    namespace Internal {

        MainWindow::MainWindow(QWidget *parent) : QsApi::PlainWindow(parent) {
            menuBar()->setProperty("core-style", true);
        }

        MainWindow::~MainWindow() {
        }

    }

}