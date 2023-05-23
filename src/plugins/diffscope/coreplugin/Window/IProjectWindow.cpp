#include "IProjectWindow.h"
#include "IProjectWindow_p.h"

#include <QDebug>
#include <QEvent>
#include <QMessageBox>

#include <QMDecoratorV2.h>
#include <QMView.h>

#include "ICore.h"

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

        auto w = new Internal::ProjectWidget();
        w->setObjectName("project-widget");

        setCentralWidget(w);
        d->cp->setParent(w);

        // Close event
        connect(d->m_doc, &IDocument::changed, d, &IProjectWindowPrivate::_q_documentChanged);
        connect(d->m_doc, &IDocument::raiseRequested, d, &IProjectWindowPrivate::_q_documentRaiseRequested);
        connect(d->m_doc, &IDocument::closeRequested, d, &IProjectWindowPrivate::_q_documentCloseRequested);
    }

    void IProjectWindow::windowAddOnsFinished() {
        Q_D(IProjectWindow);

        ICoreWindow::windowAddOnsFinished();

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
