#include "WindowElementsAdaptor.h"

namespace Core {

    static const QChar m_modifiedIdentifier[] = {0x25cf};

    class WindowElementsAdaptorPrivate {
    public:
        WindowElementsAdaptor *q;

        QWidget *w;

        bool windowModified;
        bool windowFileReadOnly;
        QString windowFilePath;
        QString windowTitle;

        WindowElementsAdaptorPrivate(WindowElementsAdaptor *q) : q(q) {
            windowModified = false;
            windowFileReadOnly = false;
        }

        void updateWindowTitle() const {
            w->setWindowTitle(q->correctWindowTitle(windowTitle));
        }
    };

    WindowElementsAdaptor::WindowElementsAdaptor(QWidget *w) : d(new WindowElementsAdaptorPrivate(this)) {
        d->w = w;
    }

    WindowElementsAdaptor::~WindowElementsAdaptor() {
    }

    QWidget *WindowElementsAdaptor::window() const {
        return d->w;
    }

    bool WindowElementsAdaptor::windowModified() const {
        return d->windowModified;
    }

    void WindowElementsAdaptor::setWindowModified(bool modified) {
        d->windowModified = modified;
        d->updateWindowTitle();
    }

    bool WindowElementsAdaptor::windowFileReadOnly() const {
        return d->windowFileReadOnly;
    }

    void WindowElementsAdaptor::setWindowFileReadOnly(bool readOnly) {
        d->windowFileReadOnly = readOnly;
        d->updateWindowTitle();
    }

    QString WindowElementsAdaptor::windowFilePath() const {
        return d->windowFilePath;
    }

    void WindowElementsAdaptor::setWindowFilePath(const QString &path) {
        d->windowFilePath = path;
        d->updateWindowTitle();
    }

    QString WindowElementsAdaptor::windowTitle() const {
        return d->windowTitle;
    }

    void WindowElementsAdaptor::setWindowTitle(const QString &title) {
        d->windowTitle = title;
        d->updateWindowTitle();
    }

    QMenuBar *WindowElementsAdaptor::menuBar() const {
        return nullptr;
    }

    void WindowElementsAdaptor::setMenuBar(QMenuBar *menuBar) {
        Q_UNUSED(menuBar);
    }

    QWidget *WindowElementsAdaptor::centralWidget() const {
        return nullptr;
    }

    void WindowElementsAdaptor::setCentralWidget(QWidget *widget) {
        Q_UNUSED(widget);
    }

    QStatusBar *WindowElementsAdaptor::statusBar() const {
        return nullptr;
    }

    void WindowElementsAdaptor::setStatusBar(QStatusBar *statusBar) {
        Q_UNUSED(statusBar);
    }

    void WindowElementsAdaptor::setWindow(QWidget *w) {
        d->w = w;
    }

    QString WindowElementsAdaptor::modifiedIdentifier() const {
        return QString(m_modifiedIdentifier, sizeof(m_modifiedIdentifier) / sizeof(QChar));
    }

    QString WindowElementsAdaptor::correctWindowTitle(const QString &title) const {
        // ReadOnly sign not used
        return d->windowModified ? QString("%1 %2").arg(modifiedIdentifier(), title) : title;
    }

}