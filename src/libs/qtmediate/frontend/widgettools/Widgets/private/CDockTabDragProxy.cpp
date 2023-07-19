#include "CDockTabDragProxy.h"
#include "CDockDragLabel.h"
#include "CDockTabBar.h"

#include "../CDockFrame.h"
#include "CDockCard_p.h"
#include "CDockFrame_p.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QTimer>

#include "QMView.h"

#include <QtMath>

#define DOCK_CARD_DRAG_GLOBAL 0

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
        for (auto &sender : senders) {
            disconnect(sender, &CDockTabBar::dragStarted, this, &CDockTabDragProxy::_q_tabDragStarted);
        }
    }

    m_frame = frame;
    if (m_frame) {
        QList<CDockTabBar *> senders = tabBars();
        for (auto &sender : senders) {
            connect(sender, &CDockTabBar::dragStarted, this, &CDockTabDragProxy::_q_tabDragStarted);
        }
    }
}

QList<CDockTabBar *> CDockTabDragProxy::tabBars() const {
    auto d = m_frame->d_func();
    return {
        d->m_leftBar->firstBar(),  d->m_leftBar->secondBar(),  d->m_topBar->firstBar(),    d->m_topBar->secondBar(),
        d->m_rightBar->firstBar(), d->m_rightBar->secondBar(), d->m_bottomBar->firstBar(), d->m_bottomBar->secondBar(),
    };
}

QList<CDockSideBar *> CDockTabDragProxy::doubleTabBars() const {
    auto d = m_frame->d_func();
    return {
        d->m_leftBar,
        d->m_topBar,
        d->m_rightBar,
        d->m_bottomBar,
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

void CDockTabDragProxy::_q_tabDragStarted(CDockCard *card, const QPoint &pos, const QPixmap &pixmap) {
    auto orgDoubleBar = card->d_func()->m_tabBar->sideBar();
    if (orgDoubleBar->count() == 1) {
        card->setDisabled(true);
        card->installEventFilter(this);
    } else {
        card->hide();
    }

#if DOCK_CARD_DRAG_GLOBAL
    m_dragger = new CDockDragLabel();
    m_dragger->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
#else
    m_dragger = new CDockDragLabel(m_frame);
    m_dragger->setWindowFlags(Qt::Popup | Qt::NoDropShadowWindowHint);
#endif
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
    if (!m_dragger->parentWidget() || (m_dragger->windowFlags() & (Qt::Window | Qt::Popup))) {
        m_dragger->move(QCursor::pos() - m_dragger->currentPos);
    } else {
        m_dragger->move(m_dragger->parentWidget()->mapFromGlobal(QCursor::pos()) - m_dragger->currentPos);
    }

    auto bars = doubleTabBars();

    CDockSideBar *targetBar = nullptr;
    for (auto bar : bars) {
        if (!targetBar) {
            int widthHint = m_dragger->currentCard->widthHint();
            bool hit = false;
            if (bar->count() == 0) {
                QPoint pos;
                QSize size;
                auto d = m_frame->d_func();
                if (bar == d->m_leftBar) {
                    pos = bar->mapToGlobal(QPoint(0, 0));
                    size = QSize(widthHint, bar->height());
                } else if (bar == d->m_rightBar) {
                    pos = bar->mapToGlobal(QPoint(bar->width() - widthHint, 0));
                    size = QSize(widthHint, bar->height());
                } else if (bar == d->m_topBar) {
                    pos = bar->mapToGlobal(QPoint(0, 0));
                    size = QSize(bar->width(), widthHint);
                } else {
                    pos = bar->mapToGlobal(QPoint(0, bar->height() - widthHint));
                    size = QSize(bar->width(), widthHint);
                }
                QRect rect(pos, size);
                hit = QMView::rectHitTest(rect, QRect(m_dragger->mapToGlobal(QPoint(0, 0)), m_dragger->size()));
            } else {
                hit = QMView::widgetHitTest(bar, m_dragger);
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
    for (auto card : cards) {
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
        auto orgBar = card->d_func()->m_tabBar;
        if (bar->orientation() == Qt::Horizontal) {
            auto leftBar = bar->firstBar();
            auto rightBar = bar->secondBar();

            int barX = bar->mapToGlobal({}).x();
            int leftPos = barX + leftBar->width();
            int rightPos = barX + bar->width() - rightBar->width();

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

            int barY = bar->mapToGlobal({}).y();
            int topPos = barY + topBar->height();
            int bottomPos = barY + bar->height() - bottomBar->height();

            orgBar->removeCard(card);
            if (m_dragger->y() - topPos < bottomPos - (m_dragger->y() + m_dragger->height())) {
                int index = cardAtWidget(topBar, m_dragger);
                topBar->insertCard(index, card);
            } else {
                int index = cardAtWidget(bottomBar, m_dragger, true);
                bottomBar->insertCard(index, card);
            }
        }
    } else {
        auto pos = QCursor::pos();
        QTimer::singleShot(0, card, [pos, card]() {
            if (card->viewMode() == CDockCard::DockPinned) {
                card->setViewMode(CDockCard::Float);
                card->moveWidget(pos);
            } else if (!card->isChecked()) {
                card->moveWidget(pos);
            }
            card->setChecked(true);
        });
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
    for (auto bar : bars) {
        bar->setHighlight(false);
    }
}
