#include "CDockFrame.h"

#include "private/CDockFrame_p.h"

#include <QEvent>

CDockFrame::CDockFrame(QWidget *parent)
    : QFrame(parent), d(new CDockFramePrivate(this)) {
    d->init();
}

CDockFrame::~CDockFrame() {
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


QAbstractButton *CDockFrame::addWidget(Qt::Edge edge, Qs::Priority number, QWidget *w) {
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
    ((number == Qs::Primary) ? bar->firstBar() : bar->secondBar())->addCard(card);
    return card;
}

void CDockFrame::removeWidget(QAbstractButton *card) {
    auto realCard = qobject_cast<CDockCard *>(card);
    if (!realCard) {
        qDebug() << "CDockFrame: invalid card pointer" << (void *) card;
        return;
    }
    auto tabBar = realCard->tabBar();
    auto doubleBar = tabBar->doubleTabBar();
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
    tabBar->removeCard(realCard);
    realCard->deleteLater();
}

QSize CDockFrame::dragAreaSize() const {
    return d->m_dragAreaSize;
}

void CDockFrame::setDragAreaSize(const QSize &dragAreaSize) {
    d->m_dragAreaSize = dragAreaSize;
    emit dragAreaSizeChanged();
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
