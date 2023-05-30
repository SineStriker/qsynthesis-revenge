#include "IProjectWindow.h"
#include "IProjectWindow_p.h"

#include <QDebug>
#include <QEvent>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

#include <CToolBar.h>
#include <CToolButton.h>

#include <QMDecoratorV2.h>
#include <QMView.h>

#include "ICore.h"

#include "Internal/Widgets/FloatingTitleBar.h"

namespace Core {

    IProjectWindowPrivate::IProjectWindowPrivate() {
        m_forceClose = false;
    }

    void IProjectWindowPrivate::init() {
        Q_Q(IProjectWindow);

        m_doc = new DspxDocument(q);
        m_doc->setAutoRemoveLogDirectory(false);
    }

    void IProjectWindowPrivate::reloadStrings() {
    }

    void IProjectWindowPrivate::reloadMainToolbar() {
        Q_Q(IProjectWindow);
        auto items = q->actionItems();
        auto bar = q->mainToolbar();

        mainToolbarCtx->buildToolBarWithState(items, bar);
    }

    void IProjectWindowPrivate::_q_documentChanged() {
        Q_Q(IProjectWindow);
        q->setWindowModified(m_doc->isModified());
        q->setWindowTitle(m_doc->displayName());
    }

    void IProjectWindowPrivate::_q_documentRaiseRequested() {
        Q_Q(IProjectWindow);
        QMView::bringWindowToForeground(q->window());
    }

    void IProjectWindowPrivate::_q_documentCloseRequested() {
        Q_Q(IProjectWindow);

        m_forceClose = true;
        q->window()->close();
    }

    DspxDocument *IProjectWindow::doc() const {
        Q_D(const IProjectWindow);
        return d->m_doc;
    }

    QToolBar *IProjectWindow::mainToolbar() const {
        Q_D(const IProjectWindow);
        return d->m_projectWidget->mainToolbar();
    }

    CDockFrame *Core::IProjectWindow::mainDock() const {
        Q_D(const IProjectWindow);
        return d->m_projectWidget->mainDock();
    }

    QAbstractButton *IProjectWindow::addFloatingPanel(const QString &id, QWidget *panel, QWidget *titleBar) {
        Q_D(IProjectWindow);

        // Wrap
        auto bar = new Internal::FloatingTitleBar();
        bar->setTitleBar(titleBar);

        auto obj = new QObject(d);
        obj->setProperty("checked", globalAttribute(id).toBool());

        connect(bar->foldButton(), &QAbstractButton::toggled, obj, [id, panel, this](bool checked) {
            panel->setVisible(checked); //
        });

        connect(bar->closeButton(), &QAbstractButton::clicked, obj, [id, this]() {
            setGlobalAttribute(id, false, true); //
        });

        connect(obj, &QObject::destroyed, obj, [panel, bar]() {
            panel->deleteLater();
            bar->deleteLater();
        });

        connect(this, &ObjectPool::globalAttributeChanged, obj,
                [id, obj, panel, bar, d](const QString &_id, const QVariant &var, const QVariant &orgVar) {
                    if (_id != id) {
                        return;
                    }

                    bool checked = var.toBool();
                    if (checked == obj->property("checked").toBool()) {
                        return;
                    }

                    if (checked) {
                        d->m_projectWidget->piano()->addWidget(panel, bar);
                        bar->foldButton()->setChecked(true);
                    } else {
                        d->m_projectWidget->piano()->removeWidget(panel);
                    }

                    obj->setProperty("checked", checked);
                });

        return bar->titleButton();
    }

    QAbstractButton *IProjectWindow::addToolWindowPanel(const QString &id, int preferredEdge, QWidget *panel) {
        return nullptr;
    }

    IProjectWindow::IProjectWindow(QObject *parent) : IProjectWindow(*new IProjectWindowPrivate(), parent) {
    }

    IProjectWindow::~IProjectWindow() {
    }

    void IProjectWindow::setupWindow() {
        Q_D(IProjectWindow);

        ICoreWindow::setupWindow();

        auto win = window();
        win->setObjectName("project-window");
        win->setAcceptDrops(true);

        d->mainToolbarCtx = ICore::instance()->actionSystem()->context("project.MainToolbar");

        auto &centralWidget = d->m_projectWidget = new Internal::ProjectWidget();
        setCentralWidget(centralWidget);
        d->cp->setParent(centralWidget);

        // Close event
        connect(d->m_doc, &IDocument::changed, d, &IProjectWindowPrivate::_q_documentChanged);
        connect(d->m_doc, &IDocument::raiseRequested, d, &IProjectWindowPrivate::_q_documentRaiseRequested);
        connect(d->m_doc, &IDocument::closeRequested, d, &IProjectWindowPrivate::_q_documentCloseRequested);
    }

    void IProjectWindow::windowAddOnsFinished() {
        Q_D(IProjectWindow);

        ICoreWindow::windowAddOnsFinished();

        connect(d->mainToolbarCtx, &ActionContext::stateChanged, d, &IProjectWindowPrivate::reloadMainToolbar);
        d->reloadMainToolbar();

        auto win = window();

        qIDec->installLocale(this, _LOC(IProjectWindowPrivate, d));
        qIDec->installTheme(win, "core.ProjectWindow");

        ICore::instance()->windowSystem()->loadWindowGeometry(metaObject()->className(), win, {1200, 800});
    }

    void IProjectWindow::windowAboutToClose() {
        Q_D(IProjectWindow);

        // *(int *) nullptr = 1;

        // Windows should be all closed before quit signal arrives except a kill or interrupt signal
        // is emitted, only when this function is called the log should be removed expectedly.
        if (!d->m_doc->isModified() || d->m_forceClose) {
            d->m_doc->setAutoRemoveLogDirectory(true);
        }

        ICore::instance()->windowSystem()->saveWindowGeometry(metaObject()->className(), window());
    }

    IProjectWindow::IProjectWindow(IProjectWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IProjectWindow::WindowTypeID(), parent) {
        d.init();
    }

}
