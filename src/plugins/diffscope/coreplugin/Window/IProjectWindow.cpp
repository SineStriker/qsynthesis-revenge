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

#include <CoreApi/ILoader.h>

#include "ICore.h"

#include "Internal/Widgets/PianoKeyWidget.h"

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
        return d->m_toolbar;
    }

    CDockFrame *Core::IProjectWindow::mainDock() const {
        Q_D(const IProjectWindow);
        return d->m_frame;
    }

    QAbstractButton *IProjectWindow::addToolWindowPanel(const QString &id, int preferredEdge, QWidget *panel) {
        Q_D(IProjectWindow);
        return d->m_frame->addWidget(Qt::Edge(preferredEdge & 0xF), QM::Priority(preferredEdge & 0xF0), panel);
    }

    PianoRoll *IProjectWindow::pianoRoll() const {
        Q_D(const IProjectWindow);
        return d->m_pianoRoll;
    }

    IProjectWindow::IProjectWindow(QObject *parent) : IProjectWindow(*new IProjectWindowPrivate(), parent) {
    }

    IProjectWindow::~IProjectWindow() {
        Q_D(IProjectWindow);
    }

    void IProjectWindow::setupWindow() {
        Q_D(IProjectWindow);

        ICoreWindow::setupWindow();

        auto win = window();
        win->setObjectName("project-window");
        win->setAcceptDrops(true);

        d->mainToolbarCtx = ICore::instance()->actionSystem()->context("project.MainToolbar");

        auto &toolbar = d->m_toolbar;
        toolbar = new CToolBar();
        toolbar->setObjectName("main-toolbar");
        toolbar->setMovable(false);
        toolbar->setFloatable(false);
        toolbar->setOrientation(Qt::Horizontal);
        toolbar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

        auto &frame = d->m_frame;
        frame = new CDockFrame();
        frame->setObjectName("main-dock");

        auto &layout = d->m_layout;
        layout = new QVBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(toolbar);
        layout->addWidget(frame, 1);

        auto &centralWidget = d->m_centralWidget;
        centralWidget = new QWidget();
        centralWidget->setAttribute(Qt::WA_StyledBackground);
        centralWidget->setLayout(layout);

        auto &m_pianoRoll = d->m_pianoRoll;
        m_pianoRoll = new PianoRoll();
        frame->setWidget(m_pianoRoll);

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

        // Restore floating panels
        d->m_pianoRoll->readSettings();

        ICore::instance()->windowSystem()->loadWindowGeometry(metaObject()->className(), win, {1200, 800});
    }

    void IProjectWindow::windowAboutToClose() {
        Q_D(IProjectWindow);

        // Windows should be all closed before quit signal arrives except a kill or interrupt signal
        // is emitted, only when this function is called the log should be removed expectedly.
        if (!d->m_doc->isModified() || d->m_forceClose) {
            d->m_doc->setAutoRemoveLogDirectory(true);
        }

        // Save floating panels state
        d->m_pianoRoll->saveSettings();

        ICore::instance()->windowSystem()->saveWindowGeometry(metaObject()->className(), window());
    }

    IProjectWindow::IProjectWindow(IProjectWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IProjectWindow::WindowTypeID(), parent) {
        d.init();
    }

}
