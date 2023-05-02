#include "ICoreWindow.h"
#include "ICoreWindow_p.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include "ICore.h"
#include "Internal/Window/MainWindow.h"

namespace Core {

    ICoreWindowPrivate::ICoreWindowPrivate() {
        mainMenuCtx = nullptr;
    }

    void ICoreWindowPrivate::init() {
    }

    void ICoreWindowPrivate::reloadMenuBar() {
        Q_Q(ICoreWindow);
        auto items = q->actionItems();
        auto bar = q->menuBar();

        mainMenuCtx->buildMenuBarWithState(items, bar);
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
            if (qApp->property("closeHomeOnOpen").toBool())
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
        Q_D(ICoreWindow);

        window()->setProperty("top-window", true);

        // Add window and menubar as basic shortcut contexts
        addShortcutContext(window());
        addShortcutContext(menuBar());

        d->mainMenuCtx = ICore::instance()->actionSystem()->context(QString("%1.MainMenu").arg(id()));
    }

    void ICoreWindow::windowAddOnsFinished() {
        Q_D(ICoreWindow);

        connect(d->mainMenuCtx, &ActionContext::stateChanged, d, &ICoreWindowPrivate::reloadMenuBar);
        d->reloadMenuBar();
    }

    ICoreWindow::ICoreWindow(ICoreWindowPrivate &d, const QString &id, QObject *parent) : IWindow(d, id, parent) {
        d.init();
    }

}