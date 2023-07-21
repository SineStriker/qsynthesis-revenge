#include "CDockToolWindow.h"
#include "private/CDockToolWindow_p.h"

#include "private/CDockCard_p.h"

#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

#include <private/qapplication_p.h>
#include <private/qshortcutmap_p.h>
#include <qpa/qwindowsysteminterface.h>

CDockToolWindowPrivate::CDockToolWindowPrivate() {
    card = nullptr;
    resizeMargins = {8, 8, 8, 8};
}

CDockToolWindowPrivate::~CDockToolWindowPrivate() {
}

void CDockToolWindowPrivate::init() {
    Q_Q(CDockToolWindow);
    q->setAttribute(Qt::WA_StyledBackground);
}

void CDockToolWindowPrivate::setCard(CDockCard *card) {
    if (this->card == card)
        return;

    if (card) {
        auto &fp = card->d_func()->m_floatingHelper;
        orgResizeMargins = fp->resizeMargins();
        fp->setResizeMargins(resizeMargins);
    } else {
        this->card->d_func()->m_floatingHelper->setResizeMargins(orgResizeMargins);
    }

    this->card = card;
}

CDockToolWindow::CDockToolWindow(QWidget *parent) : CDockToolWindow(*new CDockToolWindowPrivate(), parent) {
}

CDockToolWindow::~CDockToolWindow() {
}

QMargins CDockToolWindow::resizeMargins() const {
    Q_D(const CDockToolWindow);
    return d->resizeMargins;
}

void CDockToolWindow::setResizeMargins(const QMargins &resizeMargins) {
    Q_D(CDockToolWindow);
    d->resizeMargins = resizeMargins;

    if (d->card) {
        d->card->d_func()->m_floatingHelper->setResizeMargins(resizeMargins);
    }
}

CDockCard *CDockToolWindow::card() const {
    Q_D(const CDockToolWindow);
    return d->card;
}

QMenu *CDockToolWindow::createCardMenu() const {
    auto card = this->card();
    if (!card)
        return nullptr;

    return CDockCardPrivate::createViewModeMenu(card);
}

void CDockToolWindow::viewModeChanged(CDockCard::ViewMode viewMode) {
    Q_UNUSED(viewMode);
}

class ShortcutFilter : public QObject {
public:
    ShortcutFilter(QWidget *org) : m_org(org), m_handled(false) {
    }

    inline bool handled() const {
        return m_handled;
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override {
        if (event->type() == QEvent::Shortcut) {
            QApplicationPrivate::active_window = m_org;
            m_handled = true;
        }
        return QObject::eventFilter(watched, event);
    }

private:
    QWidget *m_org;
    bool m_handled;
};

void CDockToolWindow::keyPressEvent(QKeyEvent *event) {
    auto card = this->card();

    QWidget *window;
    if (isWindow() && card && (window = card->window())) {
        goto out;
    }
    event->ignore();
    return;

out:
    event->accept();

    // Hack `active_window` temporarily
    auto org = QApplicationPrivate::active_window;
    QApplicationPrivate::active_window = window;

    // Make sure to restore `active_window` right away if shortcut matches
    ShortcutFilter filter(org);
    qApp->installEventFilter(&filter);

    // Retransmit event
    QKeyEvent keyEvent(QEvent::ShortcutOverride, event->key(), event->modifiers(), event->nativeScanCode(),
                       event->nativeVirtualKey(), event->nativeModifiers(), event->text(), event->isAutoRepeat(),
                       event->count());
    QGuiApplicationPrivate::instance()->shortcutMap.tryShortcut(&keyEvent);

    if (!filter.handled()) {
        QApplicationPrivate::active_window = org;
    }
}

CDockToolWindow::CDockToolWindow(CDockToolWindowPrivate &d, QWidget *parent) : QWidget(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
