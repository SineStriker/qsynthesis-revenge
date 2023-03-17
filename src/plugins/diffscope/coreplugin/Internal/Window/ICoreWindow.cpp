#include "ICoreWindow.h"

#include "MainWindow.h"

namespace Core {

    namespace Internal {

        ICoreWindowFactory::ICoreWindowFactory(const QString &id) : IWindowFactory(id, CreateWithId) {
        }

        ICoreWindowFactory::~ICoreWindowFactory() {
        }

        IWindow *ICoreWindowFactory::create(QObject *parent) {
            return nullptr;
        }

        IWindow *ICoreWindowFactory::create(const QString &id, QObject *parent) {
            return new ICoreWindow(id, parent);
        }

        ICoreWindow::ICoreWindow(const QString &id, QObject *parent) : IWindow(id, parent) {
        }

        ICoreWindow::~ICoreWindow() {
        }

        QMenuBar *ICoreWindow::menuBar() const {
            return qobject_cast<Internal::MainWindow *>(window())->menuBar();
        }

        void ICoreWindow::setMenuBar(QMenuBar *menuBar) {
            IWindow::setMenuBar(menuBar);
        }

        QWidget *ICoreWindow::centralWidget() const {
            return qobject_cast<Internal::MainWindow *>(window())->centralWidget();
        }

        void ICoreWindow::setCentralWidget(QWidget *widget) {
            qobject_cast<Internal::MainWindow *>(window())->setCentralWidget(widget);
        }

        QStatusBar *ICoreWindow::statusBar() const {
            return qobject_cast<Internal::MainWindow *>(window())->statusBar();
        }

        void ICoreWindow::setStatusBar(QStatusBar *statusBar) {
            qobject_cast<Internal::MainWindow *>(window())->setStatusBar(statusBar);
        }

        QWidget *ICoreWindow::createWindow(QWidget *parent) const {
            return new Internal::MainWindow(parent);
        }
    }

}