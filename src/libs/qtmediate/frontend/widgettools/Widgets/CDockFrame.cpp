#include "CDockFrame.h"

#include "private/CDockCard_p.h"
#include "private/CDockFrame_p.h"

#include <QEvent>
#include <QMenu>

CDockFrame::CDockFrame(QWidget *parent) : CDockFrame(*new CDockFramePrivate(), parent) {
}

CDockFrame::~CDockFrame() {
}

QWidget *CDockFrame::widget() const {
    Q_D(const CDockFrame);
    return (d->m_centralContainer->count() == 0) ? nullptr : d->m_centralContainer->widget(0);
}

void CDockFrame::setWidget(QWidget *w) {
    Q_D(CDockFrame);
    if (d->m_centralContainer->count() > 0) {
        delete takeWidget();
    }
    if (w) {
        d->m_centralContainer->addWidget(w);
    }
}

QWidget *CDockFrame::takeWidget() {
    Q_D(CDockFrame);
    if (d->m_centralContainer->count() > 0) {
        auto w = d->m_centralContainer->widget(0);
        d->m_centralContainer->removeWidget(w);
        return w;
    }
    return nullptr;
}


CDockCard *CDockFrame::addWidget(Qt::Edge edge, QM::Priority number, QWidget *w) {
    Q_D(CDockFrame);
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
    auto card = createCard(w);
    card->setWidget(w);
    ((number == QM::Primary) ? bar->firstBar() : bar->secondBar())->addCard(card);

    widgetAdded(edge, number, w, card);
    emit cardAdded(card);

    return card;
}

void CDockFrame::removeWidget(CDockCard *card) {
    Q_D(CDockFrame);
    if (!card) {
        qDebug() << "CDockFrame: invalid card pointer" << (void *) card;
        return;
    }
    auto tabBar = card->d_func()->m_tabBar;
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
    Q_D(CDockFrame);
    if (!card) {
        qDebug() << "CDockFrame: invalid card pointer" << (void *) card;
        return;
    }
    auto tabBar = card->d_func()->m_tabBar;
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
    auto orgBar = card->d_func()->m_tabBar;
    orgBar->removeCard(card);
    newBar->addCard(card);
}

QList<CDockCard *> CDockFrame::widgets(Qt::Edge edge, QM::Priority number) const {
    Q_D(const CDockFrame);
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
    Q_D(const CDockFrame);
    return d->m_leftBar->isVisible();
}

void CDockFrame::setBarVisible(bool visible) {
    Q_D(CDockFrame);
    d->m_leftBar->setVisible(visible);
    d->m_topBar->setVisible(visible);
    d->m_rightBar->setVisible(visible);
    d->m_bottomBar->setVisible(visible);
}

void CDockFrame::toggleBarVisible() {
    setBarVisible(!barVisible());
}

void CDockFrame::toggleMaximize(Qt::Edge edge) {
    Q_D(CDockFrame);
    switch (edge) {
        case Qt::TopEdge: {
            auto offset = d->m_horizontalSplitter->height() - d->m_horizontalSplitter->minimumSizeHint().height();
            if (offset == 0) {
                d->m_verticalSplitter->setSizes(d->orgVSizes);
            } else {
                auto sizes = d->m_verticalSplitter->sizes();
                d->orgVSizes = sizes;
                sizes[0] += offset;
                sizes[1] -= offset;
                d->m_verticalSplitter->setSizes(sizes);
            }
            break;
        }
        case Qt::BottomEdge: {
            auto offset = d->m_horizontalSplitter->height() - d->m_horizontalSplitter->minimumSizeHint().height();
            if (offset == 0) {
                d->m_verticalSplitter->setSizes(d->orgVSizes);
            } else {
                auto sizes = d->m_verticalSplitter->sizes();
                d->orgVSizes = sizes;
                sizes[2] += offset;
                sizes[1] -= offset;
                d->m_verticalSplitter->setSizes(sizes);
            }
            break;
        }
        case Qt::LeftEdge: {
            auto offset = d->m_centralContainer->width() - d->m_centralContainer->minimumSizeHint().width();
            if (offset == 0) {
                d->m_horizontalSplitter->setSizes(d->orgHSizes);
            } else {
                auto sizes = d->m_horizontalSplitter->sizes();
                d->orgHSizes = sizes;
                sizes[0] += offset;
                sizes[1] -= offset;
                d->m_horizontalSplitter->setSizes(sizes);
            }
            break;
        }
        case Qt::RightEdge: {
            auto offset = d->m_centralContainer->width() - d->m_centralContainer->minimumSizeHint().width();
            if (offset == 0) {
                d->m_horizontalSplitter->setSizes(d->orgHSizes);
            } else {
                auto sizes = d->m_horizontalSplitter->sizes();
                d->orgHSizes = sizes;
                sizes[2] += offset;
                sizes[1] -= offset;
                d->m_horizontalSplitter->setSizes(sizes);
            }
            break;
        }
    }
}

QList<int> CDockFrame::orientationSizes(Qt::Orientation orientation) const {
    Q_D(const CDockFrame);
    switch (orientation) {
        case Qt::Horizontal: {
            auto sizes = d->m_horizontalSplitter->sizes();
            if (sizes == QList<int>{0, 0, 0}) {
                sizes = {0, d->m_horizontalSplitter->width(), 0};
            }
            return sizes;
            break;
        }
        case Qt::Vertical: {
            auto sizes = d->m_verticalSplitter->sizes();
            if (sizes == QList<int>{0, 0, 0}) {
                sizes = {0, d->m_verticalSplitter->height(), 0};
            }
            return sizes;
            break;
        }
    }
    return {};
}

void CDockFrame::setOrientationSizes(Qt::Orientation orientation, const QList<int> &sizes) {
    Q_D(CDockFrame);
    switch (orientation) {
        case Qt::Horizontal:
            d->m_horizontalSplitter->setSizes(sizes);
            break;
        case Qt::Vertical:
            d->m_verticalSplitter->setSizes(sizes);
            break;
    }
}

int CDockFrame::edgeSize(Qt::Edge edge) const {
    Q_D(const CDockFrame);
    switch (edge) {
        case Qt::TopEdge: {
            return d->m_topPanel->height();
            break;
        }
        case Qt::BottomEdge: {
            return d->m_bottomPanel->height();
            break;
        }
        case Qt::LeftEdge: {
            return d->m_leftPanel->width();
            break;
        }
        case Qt::RightEdge: {
            return d->m_rightPanel->width();
            break;
        }
    }
    return 0;
}

void CDockFrame::setEdgeSize(Qt::Edge edge, int size) {
    Q_D(CDockFrame);

    switch (edge) {
        case Qt::TopEdge: {
            auto sizes = d->m_verticalSplitter->sizes();
            auto offset = size - sizes[0];
            sizes[0] += offset;
            sizes[1] -= offset;
            d->m_verticalSplitter->setSizes(sizes);
            break;
        }
        case Qt::BottomEdge: {
            auto sizes = d->m_verticalSplitter->sizes();
            auto offset = size - sizes[2];
            d->orgVSizes = sizes;
            sizes[2] += offset;
            sizes[1] -= offset;
            d->m_verticalSplitter->setSizes(sizes);
            break;
        }
        case Qt::LeftEdge: {
            auto sizes = d->m_horizontalSplitter->sizes();
            auto offset = size - sizes[0];
            sizes[0] += offset;
            sizes[1] -= offset;
            d->m_horizontalSplitter->setSizes(sizes);
            break;
        }
        case Qt::RightEdge: {
            auto sizes = d->m_horizontalSplitter->sizes();
            auto offset = size - sizes[2];
            sizes[2] += offset;
            sizes[1] -= offset;
            d->m_horizontalSplitter->setSizes(sizes);
            break;
        }
    }
}

CDockCard *CDockFrame::createCard(QWidget *w) {
    Q_UNUSED(w)
    return new CDockCard();
}

void CDockFrame::widgetAdded(Qt::Edge edge, QM::Priority number, QWidget *w, CDockCard *card) {
}

void CDockFrame::widgetAboutToRemove(CDockCard *card) {
}

bool CDockFrame::eventFilter(QObject *obj, QEvent *event) {
    Q_D(CDockFrame);
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

CDockFrame::CDockFrame(CDockFramePrivate &d, QWidget *parent) : QFrame(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}