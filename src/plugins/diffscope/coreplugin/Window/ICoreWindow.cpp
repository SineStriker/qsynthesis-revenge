#include "ICoreWindow.h"
#include "ICoreWindow_p.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include "ICore.h"
#include "Internal/Window/MainWindow.h"

namespace Core {

    ICoreWindowPrivate::ICoreWindowPrivate() {
    }

    void ICoreWindowPrivate::init() {
    }

    QMenuBar *ICoreWindow::menuBar() const {
        return qobject_cast<Internal::MainWindow *>(window())->menuBar();
    }

    void ICoreWindow::setMenuBar(QMenuBar *menuBar) {
        qobject_cast<Internal::MainWindow *>(window())->setMenuBar(menuBar);
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

    QString ICoreWindow::correctWindowTitle(const QString &title) const {
        return IWindow::correctWindowTitle(ICore::displayTitle(title));
    }

    void ICoreWindow::openFile(const QString &path) {
        QFileInfo info(path);
        if (!info.isFile()) {
            return;
        }

        auto docMgr = ICore::instance()->documentSystem();
        auto specs = docMgr->supportedDocTypes(info.completeSuffix());
        if (specs.isEmpty()) {
            QMessageBox::critical(
                window(), ICore::mainTitle(),
                tr("Can't find editor of the file %1!").arg(QDir::toNativeSeparators(info.canonicalFilePath())));
            return;
        }

        // TODO: add editor selection method
        auto spec = specs.front();

        if (spec->open(path) && id() == "home") {
            window()->close();
        }
    }

    ICoreWindow::ICoreWindow(const QString &id, QObject *parent) : ICoreWindow(*new ICoreWindowPrivate(), id, parent) {
    }

    ICoreWindow::~ICoreWindow() {
    }

    QWidget *ICoreWindow::createWindow(QWidget *parent) const {
        return new Internal::MainWindow(parent);
    }

    void ICoreWindow::setupWindow() {
        // #ifdef Q_OS_LINUX
        //         window()->setWindowIcon(QIcon(":/svg/app/diffsinger.svg"));
        // #endif
        window()->setProperty("top-window", true);
    }

    ICoreWindow::ICoreWindow(ICoreWindowPrivate &d, const QString &id, QObject *parent) : IWindow(d, id, parent) {
        d.init();
    }

}