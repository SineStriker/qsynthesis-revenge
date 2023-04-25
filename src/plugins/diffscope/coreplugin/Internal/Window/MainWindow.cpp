#include "MainWindow.h"

#include "CMenuBar.h"

namespace Core {

    namespace Internal {

        MainWindow::MainWindow(QWidget *parent) : QsApi::PlainWindow(parent) {
            auto bar = qobject_cast<CMenuBar *>(menuBar());
            if (bar) {
                bar->setProperty("core-style", true);
                bar->extensionMenu()->setProperty("core-style", true);
            }
        }

        MainWindow::~MainWindow() {
        }

    }

}