#include "IProjectWindow.h"
#include "IProjectWindow_p.h"

#include <QDebug>
#include <QEvent>

#include <QMDecoratorV2.h>
#include <QMView.h>

#include "ICore.h"

namespace Core {

    IProjectWindowPrivate::IProjectWindowPrivate() {
    }

    void IProjectWindowPrivate::init() {
        Q_Q(IProjectWindow);
        m_doc = new DspxDocument(q);
    }

    void IProjectWindowPrivate::reloadStrings() {
    }

    void IProjectWindowPrivate::reloadMenuBar() {
        Q_Q(IProjectWindow);
        mainMenuCtx->buildMenuBarWithState(q->actionItems(), q->menuBar());
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

        d->mainMenuCtx = ICore::instance()->actionSystem()->context("project.MainMenu");

        // Close event
        connect(d->m_doc, &IDocument::changed, d, &IProjectWindowPrivate::_q_documentChanged);
        connect(d->m_doc, &IDocument::raiseRequested, d, &IProjectWindowPrivate::_q_documentRaiseRequested);
        connect(d->m_doc, &IDocument::closeRequested, win, &QWidget::close);
    }

    void IProjectWindow::windowAddOnsFinished() {
        Q_D(IProjectWindow);

        auto win = window();

        connect(d->mainMenuCtx, &ActionContext::stateChanged, d, &IProjectWindowPrivate::reloadMenuBar);
        d->reloadMenuBar();

        qIDec->installLocale(this, _LOC(IProjectWindowPrivate, d));
        qIDec->installTheme(win, "core.ProjectWindow");

        ICore::instance()->windowSystem()->loadWindowGeometry(metaObject()->className(), win, {1200, 800});
    }

    void IProjectWindow::windowAboutToClose() {
        ICore::instance()->windowSystem()->saveWindowGeometry(metaObject()->className(), window());
    }

    bool IProjectWindow::eventFilter(QObject *obj, QEvent *event) {
        if (obj == window()) {
            switch (event->type()) {
                case QEvent::Close: {
                    qDebug() << "close";
                    break;
                }
                default:
                    break;
            }
        }
        return QObject::eventFilter(obj, event);
    }

    IProjectWindow::IProjectWindow(IProjectWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IProjectWindow::WindowTypeID(), parent) {
        d.init();
    }

}
