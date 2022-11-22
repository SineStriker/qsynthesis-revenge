#include "CDockTabDragProxy.h"
#include "CDockDragLabel.h"
#include "CDockTabBar.h"

#include "../CDockFrame.h"
#include "CDockFrame_p.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

#include "EventHelper.h"
#include "ViewHelper.h"

#include <QtMath>

CDockTabDragProxy::CDockTabDragProxy(QObject *parent) : QObject(parent) {
    m_frame = nullptr;
    m_dragger = nullptr;
}

CDockTabDragProxy::~CDockTabDragProxy() {
}

CDockFrame *CDockTabDragProxy::frame() const {
    return m_frame;
}

void CDockTabDragProxy::setFrame(CDockFrame *frame) {
    if (m_frame) {
        QList<CDockTabBar *> senders = tabBars();
        for (auto it = senders.begin(); it != senders.end(); ++it) {
            disconnect(*it, &CDockTabBar::dragStarted, this, &CDockTabDragProxy::_q_tabDragStarted);
        }
    }

    m_frame = frame;
    if (m_frame) {
        QList<CDockTabBar *> senders = tabBars();
        for (auto it = senders.begin(); it != senders.end(); ++it) {
            connect(*it, &CDockTabBar::dragStarted, this, &CDockTabDragProxy::_q_tabDragStarted);
        }
    }
}

QList<CDockTabBar *> CDockTabDragProxy::tabBars() const {
    return {
        m_frame->d->m_leftBar->firstBar(),   m_frame->d->m_leftBar->secondBar(),
        m_frame->d->m_topBar->firstBar(),    m_frame->d->m_topBar->secondBar(),
        m_frame->d->m_rightBar->firstBar(),  m_frame->d->m_rightBar->secondBar(),
        m_frame->d->m_bottomBar->firstBar(), m_frame->d->m_bottomBar->secondBar(),
    };
}

QList<CDockSideBar *> CDockTabDragProxy::doubleTabBars() const {
    return {
        m_frame->d->m_leftBar,
        m_frame->d->m_topBar,
        m_frame->d->m_rightBar,
        m_frame->d->m_bottomBar,
    };
}

bool CDockTabDragProxy::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_dragger) {
        switch (event->type()) {
            case QEvent::MouseMove:
                _q_tabDragMove();
                return true;
                break;
            case QEvent::FocusOut:
            case QEvent::MouseButtonRelease:
                _q_tabDragOver();
                return true;
                break;
            default:
                break;
        }
    } else if (obj == m_dragger->currentCard) {
        if (event->type() == QEvent::Paint) {
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void CDockTabDragProxy::_q_tabDragStarted(CDockCard *card, const QPoint &pos,
                                          const QPixmap &pixmap) {
    auto orgDoubleBar = card->tabBar()->doubleTabBar();
    if (orgDoubleBar->count() == 1) {
        card->setDisabled(true);
        card->installEventFilter(this);
    } else {
        card->hide();
    }

    m_dragger = new CDockDragLabel(m_frame);
    m_dragger->currentCard = card;
    m_dragger->currentPos = pos;
    m_dragger->originBar = orgDoubleBar;
    m_dragger->targetBar = nullptr;

    m_dragger->setPixmap(pixmap);
    m_dragger->adjustSize();
    m_dragger->setFocus();
    m_dragger->installEventFilter(this);
    m_dragger->grabMouse();

    _q_tabDragMove();
    m_dragger->show();
}

void CDockTabDragProxy::_q_tabDragMove() {
    QPoint pos = m_frame->mapFromGlobal(QCursor::pos());
    m_dragger->move(pos - m_dragger->currentPos);

    auto bars = doubleTabBars();

    CDockSideBar *targetBar = nullptr;
    for (auto it = bars.begin(); it != bars.end(); ++it) {
        auto bar = *it;
        if (!targetBar) {
            int widthHint = m_dragger->currentCard->widthHint();
            bool hit = false;
            if (bar->count() == 0) {
                QPoint pos;
                QSize size;
                if (bar == m_frame->d->m_leftBar) {
                    pos = bar->mapToGlobal(QPoint(0, 0));
                    size = QSize(widthHint, bar->height());
                } else if (bar == m_frame->d->m_rightBar) {
                    pos = bar->mapToGlobal(QPoint(bar->width() - widthHint, 0));
                    size = QSize(widthHint, bar->height());
                } else if (bar == m_frame->d->m_topBar) {
                    pos = bar->mapToGlobal(QPoint(0, 0));
                    size = QSize(bar->width(), widthHint);
                } else {
                    pos = bar->mapToGlobal(QPoint(0, bar->height() - widthHint));
                    size = QSize(bar->width(), widthHint);
                }
                QRect rect(pos, size);
                hit = View::rectHitTest(
                    rect, QRect(m_dragger->mapToGlobal(QPoint(0, 0)), m_dragger->size()));
            } else {
                hit = View::widgetHitTest(bar, m_dragger);
            }
            if (hit) {
                bar->setHighlight(bar != m_dragger->originBar, widthHint);
                targetBar = bar;
            } else {
                bar->setHighlight(false);
            }
        } else {
            bar->setHighlight(false);
        }
    }
    m_dragger->targetBar = targetBar;
}

static int cardAtWidget(CDockTabBar *bar, QWidget *w, bool reverse = false) {
    auto cards = bar->cards();
    int index = 0;
    QPoint center = w->mapToGlobal(w->rect().center());
    for (auto it = cards.begin(); it != cards.end(); ++it) {
        auto card = *it;
        QPoint pos = card->mapToGlobal(QPoint(0, 0));
        if (bar->orientation() == Qt::Vertical) {
            if (reverse) {
                if (pos.y() < center.y()) {
                    break;
                }
            } else {
                if (pos.y() + card->height() > center.y()) {
                    break;
                }
            }
        } else {
            if (reverse) {
                if (pos.x() < center.x()) {
                    break;
                }
            } else {
                if (pos.x() + card->width() > center.x()) {
                    break;
                }
            }
        }
        index++;
    }
    return index;
};

void CDockTabDragProxy::_q_tabDragOver() {
    CDockCard *card = m_dragger->currentCard;

    if (m_dragger->targetBar) {
        auto bar = m_dragger->targetBar;
        auto orgBar = card->tabBar();
        if (bar->orientation() == Qt::Horizontal) {
            auto leftBar = bar->firstBar();
            auto rightBar = bar->secondBar();
            int leftPos = bar->x() + leftBar->width();
            int rightPos = bar->x() + bar->width() - rightBar->width();
            orgBar->removeCard(card);
            if (m_dragger->x() - leftPos < rightPos - (m_dragger->x() + m_dragger->width())) {
                int index = cardAtWidget(leftBar, m_dragger);
                leftBar->insertCard(index, card);
            } else {
                int index = cardAtWidget(rightBar, m_dragger, true);
                rightBar->insertCard(index, card);
            }
        } else {
            auto topBar = bar->firstBar();
            auto bottomBar = bar->secondBar();
            int topPos = bar->y() + topBar->height();
            int bottomPos = bar->y() + bar->height() - bottomBar->height();
            orgBar->removeCard(card);
            if (m_dragger->y() - topPos < bottomPos - (m_dragger->y() + m_dragger->height())) {
                int index = cardAtWidget(topBar, m_dragger);
                topBar->insertCard(index, card);
            } else {
                int index = cardAtWidget(bottomBar, m_dragger, true);
                bottomBar->insertCard(index, card);
            }
        }
    }

    if (!card->isEnabled()) {
        card->removeEventFilter(this);
        card->setDisabled(false);
        card->update();
    }
    if (card->isHidden()) {
        card->show();
    }

    m_dragger->removeEventFilter(this);
    m_dragger->releaseMouse();
    m_dragger->deleteLater(); // Don't delete directly
    m_dragger = nullptr;

    auto bars = doubleTabBars();
    for (auto it = bars.begin(); it != bars.end(); ++it) {
        auto bar = *it;
        bar->setHighlight(false);
    }
}
