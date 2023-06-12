#include "CDockFrame.h"

#include "private/CDockFrame_p.h"

#include <QEvent>

CDockFrame::CDockFrame(QWidget *parent) : QFrame(parent), d(new CDockFramePrivate(this)) {
    d->init();
}

CDockFrame::~CDockFrame() {
    delete d;
}

QWidget *CDockFrame::widget() const {
    return (d->m_centralContainer->count() == 0) ? nullptr : d->m_centralContainer->widget(0);
}

void CDockFrame::setWidget(QWidget *w) {
    if (d->m_centralContainer->count() > 0) {
        delete takeWidget();
    }
    if (w) {
        d->m_centralContainer->addWidget(w);
    }
}

QWidget *CDockFrame::takeWidget() {
    if (d->m_centralContainer->count() > 0) {
        auto w = d->m_centralContainer->widget(0);
        d->m_centralContainer->removeWidget(w);
        return w;
    }
    return nullptr;
}


CDockCard *CDockFrame::addWidget(Qt::Edge edge, QM::Priority number, QWidget *w) {
    CDockSideBar *bar;
    switch (edge) {
        case Qt::LeftEdge:
            bar = d->m_leftBar;
            break;
        case Qt::RightEdge:
            bar = d->m_rightBar;
            break;
        case Qt::TopEdge:
            bar = d->m_topBar;
            break;
        case Qt::BottomEdge:
            bar = d->m_bottomBar;
            break;
    }
    auto card = new CDockCard();
    card->setWidget(w);
    ((number == QM::Primary) ? bar->firstBar() : bar->secondBar())->addCard(card);

    widgetAdded(edge, number, w, card);
    emit cardAdded(card);

    return card;
}

void CDockFrame::removeWidget(CDockCard *card) {
    if (!card) {
        qDebug() << "CDockFrame: invalid card pointer" << (void *) card;
        return;
    }
    auto tabBar = card->tabBar();
    auto doubleBar = tabBar->sideBar();
    if (doubleBar == d->m_leftBar) {
        goto out;
    }
    if (doubleBar == d->m_rightBar) {
        goto out;
    }
    if (doubleBar == d->m_topBar) {
        goto out;
    }
    if (doubleBar == d->m_bottomBar) {
        goto out;
    }
    qDebug() << "CDockFrame: unidentified card" << (void *) card;
    return;

out:
    widgetAboutToRemove(card);
    emit cardAboutToRemove(card);

    tabBar->removeCard(card);
    card->deleteLater();
}

void CDockFrame::moveWidget(CDockCard *card, Qt::Edge edge, QM::Priority number) {
    if (!card) {
        qDebug() << "CDockFrame: invalid card pointer" << (void *) card;
        return;
    }
    auto tabBar = card->tabBar();
    auto doubleBar = tabBar->sideBar();
    if (doubleBar == d->m_leftBar) {
        goto out;
    }
    if (doubleBar == d->m_rightBar) {
        goto out;
    }
    if (doubleBar == d->m_topBar) {
        goto out;
    }
    if (doubleBar == d->m_bottomBar) {
        goto out;
    }
    qDebug() << "CDockFrame: unidentified card" << (void *) card;
    return;

out:
    CDockSideBar *bar;
    switch (edge) {
        case Qt::LeftEdge:
            bar = d->m_leftBar;
            break;
        case Qt::RightEdge:
            bar = d->m_rightBar;
            break;
        case Qt::TopEdge:
            bar = d->m_topBar;
            break;
        case Qt::BottomEdge:
            bar = d->m_bottomBar;
            break;
    }
    auto newBar = (number == QM::Primary) ? bar->firstBar() : bar->secondBar();
    auto orgBar = card->tabBar();
    orgBar->removeCard(card);
    newBar->addCard(card);
}

QList<CDockCard *> CDockFrame::widgets(Qt::Edge edge, QM::Priority number) {
    CDockSideBar *bar;
    switch (edge) {
        case Qt::LeftEdge:
            bar = d->m_leftBar;
            break;
        case Qt::RightEdge:
            bar = d->m_rightBar;
            break;
        case Qt::TopEdge:
            bar = d->m_topBar;
            break;
        case Qt::BottomEdge:
            bar = d->m_bottomBar;
            break;
    }
    auto tabBar = (number == QM::Primary) ? bar->firstBar() : bar->secondBar();
    return tabBar->cards();
}

bool CDockFrame::barVisible() const {
    return d->m_leftBar->isVisible();
}

void CDockFrame::setBarVisible(bool visible) {
    d->m_leftBar->setVisible(visible);
    d->m_topBar->setVisible(visible);
    d->m_rightBar->setVisible(visible);
    d->m_bottomBar->setVisible(visible);
}

void CDockFrame::toggleBarVisible() {
    setBarVisible(!barVisible());
}

void CDockFrame::widgetAdded(Qt::Edge edge, QM::Priority number, QWidget *w, CDockCard *card) {
}

void CDockFrame::widgetAboutToRemove(CDockCard *card) {
}

bool CDockFrame::eventFilter(QObject *obj, QEvent *event) {
    if (obj == d->m_leftBar) {
        switch (event->type()) {
            case QEvent::Hide:
                if (!isHidden()) {
                    break;
                }
            case QEvent::Show:
                emit barVisibleToggled(event->type() == QEvent::Show);
                break;
            default:
                break;
        }
    }
    return QWidget::eventFilter(obj, event);
}