#include "CCoupleTabDragProxy.h"
#include "CCoupleTabDragLabel.h"
#include "../CCoupleTabFrame.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

#include "EventHelper.h"
#include "ViewHelper.h"

#include <QtMath>

CCoupleTabDragProxy::CCoupleTabDragProxy(QObject *parent) : QObject(parent) {
    m_frame = nullptr;
    m_dragger = nullptr;
}

CCoupleTabDragProxy::~CCoupleTabDragProxy() {
}

CCoupleTabFrame *CCoupleTabDragProxy::frame() const {
    return m_frame;
}

void CCoupleTabDragProxy::setFrame(CCoupleTabFrame *frame) {
    if (m_frame) {
        QList<CCoupleTabBar *> senders = tabBars();
        for (auto it = senders.begin(); it != senders.end(); ++it) {
            disconnect(*it, &CCoupleTabBar::dragStarted, this,
                       &CCoupleTabDragProxy::handleTabDragStarted);
        }
    }

    m_frame = frame;
    if (m_frame) {
        QList<CCoupleTabBar *> senders = tabBars();
        for (auto it = senders.begin(); it != senders.end(); ++it) {
            connect(*it, &CCoupleTabBar::dragStarted, this,
                    &CCoupleTabDragProxy::handleTabDragStarted);
        }
    }
}

QList<CCoupleTabBar *> CCoupleTabDragProxy::tabBars() const {
    return {
        m_frame->leftBar()->firstBar(),   m_frame->leftBar()->secondBar(),
        m_frame->topBar()->firstBar(),    m_frame->topBar()->secondBar(),
        m_frame->rightBar()->firstBar(),  m_frame->rightBar()->secondBar(),
        m_frame->bottomBar()->firstBar(), m_frame->bottomBar()->secondBar(),
    };
}

QList<CCoupleTabDoubleBar *> CCoupleTabDragProxy::doubleTabBars() const {
    return {
        m_frame->leftBar(),
        m_frame->topBar(),
        m_frame->rightBar(),
        m_frame->bottomBar(),
    };
}

bool CCoupleTabDragProxy::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_dragger) {
        switch (event->type()) {
        case QEvent::MouseMove:
            handleTabDragMove();
            return true;
            break;
        case QEvent::FocusOut:
        case QEvent::MouseButtonRelease:
            handleTabDragOver();
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

void CCoupleTabDragProxy::handleTabDragStarted(CCoupleTabBarCard *card, const QPoint &pos,
                                               const QPixmap &pixmap) {
    auto orgDoubleBar = card->tabBar()->doubleTabBar();
    if (orgDoubleBar->count() == 1) {
        card->setDisabled(true);
        card->installEventFilter(this);
    } else {
        card->hide();
    }

    m_dragger = new CCoupleTabDragLabel(m_frame);
    m_dragger->currentCard = card;
    m_dragger->currentPos = pos;
    m_dragger->originBar = orgDoubleBar;
    m_dragger->targetBar = nullptr;

    m_dragger->setPixmap(pixmap);
    m_dragger->adjustSize();
    m_dragger->setFocus();
    m_dragger->installEventFilter(this);
    m_dragger->grabMouse();

    handleTabDragMove();
    m_dragger->show();
}

void CCoupleTabDragProxy::handleTabDragMove() {
    QPoint pos = m_frame->mapFromGlobal(QCursor::pos());
    m_dragger->move(pos - m_dragger->currentPos);

    auto bars = doubleTabBars();

    CCoupleTabDoubleBar *targetBar = nullptr;
    for (auto it = bars.begin(); it != bars.end(); ++it) {
        auto bar = *it;
        if (!targetBar) {
            int widthHint = m_dragger->currentCard->widthHint();
            bool hit = false;
            if (bar->count() == 0) {
                QPoint pos;
                QSize size;
                if (bar == m_frame->leftBar()) {
                    pos = bar->mapToGlobal(QPoint(0, 0));
                    size = QSize(widthHint, bar->height());
                } else if (bar == m_frame->rightBar()) {
                    pos = bar->mapToGlobal(QPoint(bar->width() - widthHint, 0));
                    size = QSize(widthHint, bar->height());
                } else if (bar == m_frame->topBar()) {
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

static int cardAtWidget(CCoupleTabBar *bar, QWidget *w, bool reverse = false) {
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

void CCoupleTabDragProxy::handleTabDragOver() {
    CCoupleTabBarCard *card = m_dragger->currentCard;

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
