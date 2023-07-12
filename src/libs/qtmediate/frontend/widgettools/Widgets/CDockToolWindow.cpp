#include "CDockToolWindow.h"
#include "private/CDockToolWindow_p.h"

#include "private/CDockCard_p.h"

#include <QEvent>

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
}

CDockToolWindow::CDockToolWindow(CDockToolWindowPrivate &d, QWidget *parent) : QWidget(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
