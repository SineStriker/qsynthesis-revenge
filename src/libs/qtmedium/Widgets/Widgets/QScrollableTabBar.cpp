#include "QScrollableTabBar.h"
#include "QScrollableTabWidget.h"
#include "private/QScrollableTabBar_p.h"

#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QTimerEvent>

QScrollableTabBar::QScrollableTabBar(QWidget *parent)
    : QScrollableTabBar(*new QScrollableTabBarPrivate(), parent) {
}

QScrollableTabBar::~QScrollableTabBar() {
}

int QScrollableTabBar::addTab(const QString &text) {
    return insertTab(count(), text);
}

int QScrollableTabBar::addTab(const QIcon &icon, const QString &text) {
    return insertTab(count(), icon, text);
}

int QScrollableTabBar::insertTab(int index, const QString &text) {
    return insertTab(index, QIcon(), text);
}

int QScrollableTabBar::insertTab(int index, const QIcon &icon, const QString &text) {
    auto tab = new QScrollableTabBarTab();
    tab->installEventFilter(this);
    tab->setIcon(icon);
    tab->setText(text);

    Q_D(QScrollableTabBar);
    d->entityLayout->insertWidget(qMin(d->entityLayout->count(), index), tab);

    connect(tab->closeButton(), &QAbstractButton::clicked, this, &QScrollableTabBar::_q_closeTab);

    int res = d->entityLayout->indexOf(tab);
    tabInserted(res);
    if (count() == 1) {
        setCurrentIndex(0);
    }
    d->updateVisibility();
    return res;
}

void QScrollableTabBar::removeTab(int index) {
    Q_D(QScrollableTabBar);
    auto item = d->entityLayout->takeAt(index);
    if (!item) {
        return;
    }

    auto tab = qobject_cast<QScrollableTabBarTab *>(item->widget());
    tab->hide();
    tab->deleteLater();
    delete item;

    if (d->previous == tab) {
        d->previous = nullptr;
    }

    int cnt = count();

    if (d->current == tab) {
        d->current = nullptr;
        switch (d->selectionBehaviorOnRemove) {
            case QTabBar::SelectPreviousTab:
                if (d->previous) {
                    d->setCurrentTab(d->previous);
                    break;
                }
            case QTabBar::SelectLeftTab: {
                if (index > 0) {
                    setCurrentIndex(index - 1);
                } else if (cnt > 0) {
                    setCurrentIndex(0);
                }
                break;
            }
            case QTabBar::SelectRightTab: {
                if (index < cnt - 1) {
                    setCurrentIndex(index + 1);
                } else if (cnt > 0) {
                    setCurrentIndex(cnt - 1);
                }
                break;
            }
        }
    }
    d->updateVisibility();

    if (cnt == 0) {
        emit currentChanged(-1, index);
    }

    tabRemoved(index);
}

void QScrollableTabBar::moveTab(int from, int to) {
    Q_D(QScrollableTabBar);
    auto item = d->entityLayout->takeAt(from);
    if (!item) {
        return;
    }
    d->entityLayout->insertItem(qMin(d->entityLayout->count(), to), item);
    emit tabMoved(from, to);
}

QString QScrollableTabBar::tabText(int index) const {
    Q_D(const QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return QString();
    }
    return tab->text();
}

void QScrollableTabBar::setTabText(int index, const QString &text) {
    Q_D(QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    tab->setText(text);
}

QIcon QScrollableTabBar::tabIcon(int index) const {
    Q_D(const QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return QIcon();
    }
    return tab->icon();
}

void QScrollableTabBar::setTabIcon(int index, const QIcon &icon) {
    Q_D(QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    tab->setIcon(icon);
}

QString QScrollableTabBar::tabToolTip(int index) const {
    Q_D(const QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return QString();
    }
    return tab->toolTip();
}

void QScrollableTabBar::setTabToolTip(int index, const QString &tip) {
    Q_D(QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    tab->setToolTip(tip);
}

QVariant QScrollableTabBar::tabData(int index) const {
    Q_D(const QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return QVariant();
    }
    return tab->data();
}

void QScrollableTabBar::setTabData(int index, const QVariant &data) {
    Q_D(QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    tab->setData(data);
}

QRect QScrollableTabBar::tabRect(int index) const {
    Q_D(const QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return QRect();
    }
    return QRect(d->entity->pos() + tab->pos(), tab->size());
}

int QScrollableTabBar::tabAt(const QPoint &pos) const {
    const int max = count();
    for (int i = 0; i < max; ++i) {
        if (tabRect(i).contains(pos)) {
            return i;
        }
    }
    return -1;
}

int QScrollableTabBar::totalWidth() const {
    Q_D(const QScrollableTabBar);
    return d->entity->width();
}

int QScrollableTabBar::currentIndex() const {
    Q_D(const QScrollableTabBar);
    return d->entityLayout->indexOf(d->current);
}

int QScrollableTabBar::count() const {
    Q_D(const QScrollableTabBar);
    return d->entityLayout->count();
}

QSize QScrollableTabBar::sizeHint() const {
    Q_D(const QScrollableTabBar);
    return QSize(QWidget::sizeHint().width(), d->entity->sizeHint().height());
}

QSize QScrollableTabBar::minimumSizeHint() const {
    return this->sizeHint();
}

QSize QScrollableTabBar::iconSize(int index) const {
    Q_D(const QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return QSize();
    }
    return tab->iconSize();
}

void QScrollableTabBar::setIconSize(int index, const QSize &size) {
    Q_D(QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    tab->setIconSize(size);
}

QTabBar::SelectionBehavior QScrollableTabBar::selectionBehaviorOnRemove() const {
    Q_D(const QScrollableTabBar);
    return d->selectionBehaviorOnRemove;
}

void QScrollableTabBar::setSelectionBehaviorOnRemove(QTabBar::SelectionBehavior behavior) {
    Q_D(QScrollableTabBar);
    d->selectionBehaviorOnRemove = behavior;
}

void QScrollableTabBar::setCurrentIndex(int index) {
    Q_D(QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    d->setCurrentTab(tab);
}

void QScrollableTabBar::tabInserted(int index) {
    Q_UNUSED(index)
}

void QScrollableTabBar::tabRemoved(int index) {
    Q_UNUSED(index)
}

void QScrollableTabBar::resizeEvent(QResizeEvent *event) {
    Q_D(QScrollableTabBar);
    d->layoutScroll();
    d->updateScroll();

    // Save Time
    if (event->oldSize().width() != event->size().width() && d->scrollBar->isVisible()) {
        d->lastResized = QTime::currentTime();
    }

    QFrame::resizeEvent(event);
}

void QScrollableTabBar::wheelEvent(QWheelEvent *event) {
    Q_D(QScrollableTabBar);
    if (d->scrollBar->isVisible()) {
        QApplication::sendEvent(d->scrollBar, event);
    }
    QFrame::wheelEvent(event);
}

void QScrollableTabBar::timerEvent(QTimerEvent *event) {
    Q_D(QScrollableTabBar);
    if (event->timerId() == d->timerId) {
        if (d->scrollBar->isVisible()) {
            if ((d->lastResized.isValid() && d->lastResized.msecsTo(QTime::currentTime()) < 1500) ||
                d->scrollBar->isSliderDown() ||
                this->rect().contains(this->mapFromGlobal(QCursor::pos()))) {
                if (!d->underMouse) {
                    d->underMouse = true;
                    d->runOpacityTween(true);
                }
            } else {
                if (d->underMouse) {
                    d->underMouse = false;
                    d->runOpacityTween(false);
                }
            }
        }
    }
}

bool QScrollableTabBar::eventFilter(QObject *obj, QEvent *event) {
    Q_D(QScrollableTabBar);

    if (qstrcmp(obj->metaObject()->className(), "QScrollableTabBarTab") == 0) {
        auto tab = qobject_cast<QScrollableTabBarTab *>(obj);
        switch (event->type()) {
            case QEvent::MouseButtonPress: {
                QMouseEvent *e = static_cast<QMouseEvent *>(event);
                if (e->button() == Qt::LeftButton) {
                    d->setCurrentTab(tab);
                }
                break;
            }
            case QEvent::MouseMove: {
                QMouseEvent *e = static_cast<QMouseEvent *>(event);
                if (e->buttons() & Qt::LeftButton) {
                    // Release Mouse
                    QMouseEvent newEvent(QEvent::MouseButtonRelease, e->pos(), Qt::LeftButton,
                                         Qt::NoButton, Qt::NoModifier);
                    QApplication::sendEvent(this, &newEvent);
                    // Start Drag
                    d->draggedIndex = d->entityLayout->indexOf(tab);
                    d->startDrag(tab);
                    d->draggedIndex = -1;
                }
                break;
            }
            case QEvent::MouseButtonRelease: {
                if (d->draggedIndex < 0) {
                    QMouseEvent *e = static_cast<QMouseEvent *>(event);
                    emit tabBarClicked(e->button(), d->entityLayout->indexOf(tab));
                }
                break;
            }
            default:
                break;
        }
    } else if (obj == d->scrollBar) {
        switch (event->type()) {
            case QEvent::Show: {
                d->layoutScroll();
                break;
            }
            case QEvent::Resize: {
                if (d->scrollBar->isVisible()) {
                    d->layoutScroll();
                }
                break;
            }
            default:
                break;
        }
    } else if (obj == d->entity) {
        switch (event->type()) {
            case QEvent::Resize: {
                auto e = static_cast<QResizeEvent *>(event);
                d->updateScroll();
                if (e->oldSize().height() != e->size().height()) {
                    updateGeometry();
                }
                break;
            }
            case QEvent::LayoutRequest: {
                d->entity->adjustSize();
                if (d->needAutoScroll) {
                    d->autoScrollToCurrent();
                    d->needAutoScroll = false;
                }
                break;
            }
            default:
                break;
        }
    }

    return QFrame::eventFilter(obj, event);
}

QScrollableTabBar::QScrollableTabBar(QScrollableTabBarPrivate &d, QWidget *parent)
    : QFrame(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}

void QScrollableTabBar::_q_scrollBarValueChanged(int value) {
    Q_D(QScrollableTabBar);
    d->entity->move(-value, d->entity->y());
}

void QScrollableTabBar::_q_scrollBarRangeChanged(int min, int max) {
    Q_UNUSED(min);
    Q_UNUSED(max);
}

void QScrollableTabBar::_q_closeTab() {
    QObject *obj = sender();
    int index = -1;

    Q_D(QScrollableTabBar);
    for (int i = 0; i < d->entityLayout->count(); ++i) {
        if (obj->parent() == d->entityLayout->itemAt(i)->widget()) {
            index = i;
            break;
        }
    }

    if (index < 0) {
        return;
    }

    emit tabCloseRequested(index);
}

double QScrollableTabBar::scrollOpacity() const {
    Q_D(const QScrollableTabBar);
    return d->opacityEffect->opacity();
}

void QScrollableTabBar::setScrollOpacity(double opacity) {
    Q_D(QScrollableTabBar);
    d->opacityEffect->setOpacity(opacity);
    emit styleChanged();
}

bool QScrollableTabBar::autoHide() const {
    Q_D(const QScrollableTabBar);
    return d->autoHide;
}

void QScrollableTabBar::setAutoHide(bool autoHide) {
    Q_D(QScrollableTabBar);
    d->autoHide = autoHide;
    d->updateVisibility();
}

QScrollBar *QScrollableTabBar::scrollBar() const {
    Q_D(const QScrollableTabBar);
    return d->scrollBar;
}

QScrollableTabBar *QScrollableTabBar::currentDraggedTabBar() {
    return QScrollableTabBarPrivate::draggedTabBar;
}

QAbstractButton *QScrollableTabBar::closeButton(int index) const {
    Q_D(const QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return nullptr;
    }
    return tab->closeButton();
}

void QScrollableTabBar::setCloseButton(int index, QAbstractButton *button) {
    Q_D(QScrollableTabBar);
    auto tab = d->tabAtIndex(index);
    if (!tab) {
        return;
    }
    tab->setCloseButton(button);
    connect(button, &QAbstractButton::clicked, this, &QScrollableTabBar::_q_closeTab);
}

int QScrollableTabBar::currentDraggedIndex() {
    Q_D(const QScrollableTabBar);
    return d->draggedIndex;
}
