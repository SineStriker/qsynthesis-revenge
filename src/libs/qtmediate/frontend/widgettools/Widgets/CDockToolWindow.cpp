#include "CDockToolWindow.h"
#include "private/CDockToolWindow_p.h"

#include "private/CDockCard_p.h"

CDockToolWindowPrivate::CDockToolWindowPrivate() {
    card = nullptr;
}

CDockToolWindowPrivate::~CDockToolWindowPrivate() {
}

void CDockToolWindowPrivate::init() {
}

CDockToolWindow::CDockToolWindow(QWidget *parent) : CDockToolWindow(*new CDockToolWindowPrivate(), parent) {
}

CDockToolWindow::~CDockToolWindow() {
}

CDockCard *CDockToolWindow::card() const {
    Q_D(const CDockToolWindow);
    return d->card;
}

void CDockToolWindow::setCard(CDockCard *card) {
    Q_D(CDockToolWindow);
    d->card = card;
}

QMenu *CDockToolWindow::cardMenu() const {
    return CDockCardPrivate::createViewModeMenu(card());
}

CDockToolWindow::CDockToolWindow(CDockToolWindowPrivate &d, QWidget *parent) : QFrame(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
